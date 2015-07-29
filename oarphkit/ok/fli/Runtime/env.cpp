/*
 * Copyright 2015 Maintainers of OarphKit
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ok/fli/Runtime/env.hpp"

#include <limits>
#include <sstream>

#include "ok/StringUtils.hpp"

namespace ok { namespace fli {

bool env::IsEnvVar(const std::string &varname) {
  return StartsWith(varname, std::string("env."));
}

env env::EmptyClone() const {
  env clone;

  if (vars.HasKey("env.anon_counter")) {
    clone.vars.SetKey<EnvAnonCounterAttr>(
        "env.anon_counter",
        EnvAnonCounter());
    clone.vars.GetValueRef<EnvAnonCounterAttr>("env.anon_counter").val =
      vars.GetValue<EnvAnonCounterAttr>("env.anon_counter").val.load();
  }

  clone.GetRegistryRef().UnionWith(GetRegistry());

  if (vars.HasKey("env.encoding_prefs")) {
    clone.vars.SetKey<EncodingPrefsFuncAttr>(
      "env.encoding_prefs",
      std::unordered_set<std::string>(
        vars.GetValue<EncodingPrefsFuncAttr>(
          "env.encoding_prefs")));
  }

  if (vars.HasKey("env.main")) {
    clone.vars.SetKey<EnvMainAttr>(
      "env.main",
      vars.GetValue<EnvMainAttr>("env.main"));
  }

  return clone;
}

void env::Union(env &&other) {
  GetRegistryRef().UnionWith(other.GetRegistry());

  other.MoveVarsTo(other.vars.GetKeys(), *this);

  if (other.vars.HasKey("env.encoding_prefs")) {
    if (!vars.HasKey("env.encoding_prefs")) {
      vars.SetKey<EncodingPrefsFuncAttr>(
          "env.encoding_prefs",
          std::unordered_set<std::string>());
    }
    const auto &other_encs =
        other.vars.GetValue<EncodingPrefsFuncAttr>("env.encoding_prefs");
    auto &this_encs =
        vars.GetValueRef<EncodingPrefsFuncAttr>("env.encoding_prefs");
    this_encs.insert(other_encs.begin(), other_encs.end());
  } else {
    // Union of all with any set = all
    vars.ClearKey("env.encoding_prefs");
  }

  if (other.vars.HasKey("env.main")) {
    vars.SetKey<EnvMainAttr>(
      "env.main",
      other.vars.GetValue<EnvMainAttr>("env.main"));
  }
}

FunctorBase::Ptr /* weak */ env::GetFunctor(const std::string &varname) {
  if (!OKASSERT_CHECK(
         vars.HasKey(varname),
         "Warn: tried to fetch non-existant functor: " + varname)) {
    return NullTUPtr<FunctorBase>();
  }

  // TODO make part of a CT check? i.e. OK_FLI_COMPILETIME_TYPECHECK_ENABLED
  if (!OKASSERT_CHECK(
        IsFunctor(varname),
        "Programming error: var is not a functor " + varname)) {
    return NullTUPtr<FunctorBase>();
  }

  auto &entry = vars.GetValueRef<EnvFunctorEntry>(varname);
  if (entry.IsCreated()) {
    return entry.GetFunctor();
  } else {
    return entry.CreateAndGetFunctor(*this);
  }
}

bool env::DecodeAllFunctors(bool include_lazy) {
  const auto &keys = vars.GetKeys();
  for (const auto &k : keys) {
    if (IsFunctor(k)) {
      const auto &entry = vars.GetValue<EnvFunctorEntry>(k);
      bool is_lazy =
         entry.GetAttrs().HasAttr<IsLazilyCreatedFuncAttr>() &&
         entry.GetAttrs().GetAttr<IsLazilyCreatedFuncAttr>();
      if (is_lazy && !include_lazy) {
        OKLOG_DEBUG("Skipping Functor " << k << " which is lazy");
        continue;
      }

      if (!entry.IsCreated()) {
        auto f = GetFunctor(k); // Best way to force-create a functor
        if (!OKASSERT_CHECK(f, "Failed to decode functor " + k)) {
          return false;
        }
      }
    }
  }
  return true;
}

bool env::EncodeAllFunctors() {
  const auto &keys = vars.GetKeys();
  for (const auto &k : keys) {
    if (IsFunctor(k)) {
      auto &entry = vars.GetValueRef<EnvFunctorEntry>(k);
      bool encoded = entry.EncodeFunctor(*this);
      if (!OKASSERT_CHECK(encoded, "Failed to encode Functor at var " + k)) {
        return false;
      }
    }
  }
  return true;
}

FunctorBase::Ptr env::CreateFunctor(SVStruct &&func_attrs) {
  std::string varname;
  bool is_main = false;

  #if OK_ENABLE_PROTOBUF
  if (func_attrs.HasAttr<PBFuncAttr>()) {
    ok_msg::Func &func_spec = func_attrs.GetAttrRef<PBFuncAttr>();
    if (func_spec.has_alias()) {
      // We're going to create a Functor
      if (func_spec.has_name()) {
        varname = func_spec.name();
        if (varname == "env.main") {
          is_main = true;
          varname = CreateDistinctAnonName("anon.func.");
          OKLOG_DEBUG(
            "Found functor requesting to be main, renaming to: " << varname);
        } else if (IsEnvVar(varname)) {
          OKLOG_ERROR(
            "Functor making invalid request for env varname, skipping: \n" <<
            PBFactory::AsTextFormatString(func_spec));
          return NullTUPtr<FunctorBase>();
        }
      }
    } else if (func_spec.has_name()) {
      // The attrs indicates a reference to an existing Functor
      return GetFunctor(func_spec.name());
    }
  }
  #endif

  if (varname.empty()) { varname = CreateDistinctAnonName("anon.func."); }

  if (vars.HasKey(varname)) {
    OKLOG(
      "Warning: overwriting existing Functor entry for varname " <<
      varname << "; functor entry: " <<
      vars.GetValue<EnvFunctorEntry>(varname).ToString());
    vars.ClearKey(varname);
  }

  vars.SetKey<EnvFunctorEntry>(varname, EnvFunctorEntryWrapper());
  vars.GetValueRef<EnvFunctorEntry>(varname).SetAttrs(std::move(func_attrs));
  auto f = GetFunctor(varname);
  if (!OKASSERT_CHECK(
         f,
         "Creation of functor from attrs failed: " +
           vars.GetValue<EnvFunctorEntry>(varname).ToString())) {

    // Clean up
    vars.ClearKey(varname);
    return NullTUPtr<FunctorBase>();
  }

  if (is_main) { SetMain(MakeUnOwned(f.get())); }
  return f;
}

void env::AssignFunctor(const std::string &varname, FunctorBase::Ptr &&f) {
  if (!vars.HasKey(varname)) {
    vars.SetKey<EnvFunctorEntry>(varname, EnvFunctorEntryWrapper());
  } else if (!IsFunctor(varname)) {
    OKLOG(
      "Warning: overwriting non-functor entry for var " <<
      varname << " " <<
      "with functor: " << f->ToString());
    vars.ClearKey(varname);
    vars.SetKey<EnvFunctorEntry>(varname, EnvFunctorEntryWrapper());
  }
  vars.GetValueRef<EnvFunctorEntry>(varname).SetFunctor(std::move(f));
}

std::string env::CreateDistinctAnonName(const std::string &prefix) {
  if (!vars.HasKey("env.anon_counter")) {
    vars.SetKey<EnvAnonCounterAttr>("env.anon_counter", EnvAnonCounter());
  }

  uint64_t suffix_id = 0;
  do {
    suffix_id =
        vars.GetValueRef<EnvAnonCounterAttr>("env.anon_counter")
          .val.fetch_add(1, std::memory_order_relaxed);
    std::string varname = prefix + std::to_string(suffix_id);
    if (!vars.HasKey(varname)) { return varname; }
  } while (suffix_id < std::numeric_limits<uint64_t>::max());

  // If we get here, WAT
  OKLOG_ERROR("Error: exhausted distinct anon variable names!!");
  return prefix;
}

bool env::RunMain() {
  auto f = GetMain();
  if (!OKASSERT_CHECK(f, "No main to run")) {
    return false;
  }
  f->Call(rt_datumptr()); // TODO: if ret type is bool or int, forward it
  return true;
}

bool env::SetMain(FunctorBase::Ptr /* weak */ f) {
  if (!OKASSERT_CHECK(f, "Can't set main to null; use ClearMain()?")) {
    return false;
  }

  auto f_to_name = GetFunctorToName();
  auto entry = f_to_name.find(f.get());
  if (!OKASSERT_CHECK(
        entry != f_to_name.end(),
        "Env does not know of Functor: " + f->ToString())) {
    return false;
  }
  vars.SetKey<EnvMainAttr>("env.main", entry->second);
  return true;
}

bool env::SetMain(const std::string &varname) {
  if (!OKASSERT_CHECK(IsFunctor(varname), "Not a functor: " + varname)) {
    return false;
  }
  vars.SetKey<EnvMainAttr>("env.main", varname);
  return true;
}

void env::ClearMain() {
  vars.ClearKey("env.main");
}

FunctorBase::Ptr /* weak */ env::GetMain() {
  if (vars.HasKey("env.main")) {
    const auto &func_name = vars.GetValue<EnvMainAttr>("env.main");
    return GetFunctor(func_name);
  } else {
    return NullTUPtr<FunctorBase>();
  }
}



#if OK_ENABLE_PROTOBUF
bool env::FromPBMessage(ok_msg::EnvState &m) {
  return vars.FromPBMessage(*m.mutable_vars());
}

bool env::ToPBMessage(ok_msg::EnvState &m) {
  return vars.ToPBMessage(*m.mutable_vars());
}
#endif

std::string env::ToString() const {
  std::stringstream ss;

  ss << "FLi Env:\n\n";
  ss << vars.ToString();
  ss << "\n\n";

  return ss.str();
}

} /* namespace fli */
} /* namespace ok */
