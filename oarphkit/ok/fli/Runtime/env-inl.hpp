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
#ifndef OK_FLI_RUNTIME_ENV_INL_HPP_
#define OK_FLI_RUNTIME_ENV_INL_HPP_

#ifndef OK_FLI_ENV_HPP_
#error "Include env.hpp instead"
#endif

#include "ok/BranchPredict.hpp"
#include "ok/fli/Runtime/EnvFunctorEntryWrapper.hpp"

namespace ok { namespace fli {

inline
env::env() {
  vars.RegisterEntry<EnvAnonCounterAttr>();
  vars.RegisterEntry<EnvFunctorEntry>();
  vars.RegisterEntry<EnvMainAttr>();

  vars.SetKey<EnvRegistry>("env.root_registry", Registry());
  vars
    .GetValueRef<EnvRegistry>("env.root_registry")
      .SetName("env.root_registry");
}

inline
bool env::IsFunctor(const std::string &varname) const {
  return vars.HasKey(varname) &&
      vars.GetValueType(varname) == EnvFunctorEntry::EntryTypename();
}

// Tell the functor at `varname` to encode as `EntryT` (e.g. PBFuncAttr)
template <typename EntryT>
inline
void env::AddFunctorEncodingPref() {
  if (!vars.HasKey("env.encoding_prefs")) {
    vars.SetKey<EncodingPrefsFuncAttr>(
        "env.encoding_prefs",
        std::unordered_set<std::string>());
  }
  vars
    .GetValueRef<EncodingPrefsFuncAttr>("env.encoding_prefs")
    .insert(EntryT::EntryTypename());
}

template <typename EntryT>
inline
bool env::HasEncoding() const {
  if (OK_LIKELY(!vars.HasKey("env.encoding_prefs"))) { return true; }

  const auto &encs =
      vars.GetValue<EncodingPrefsFuncAttr>("env.encoding_prefs");
  return encs.find(EntryT::EntryTypename()) != encs.end();
}

inline
std::unordered_map<FunctorBase *, std::string>
env::GetFunctorToName() const {
  std::unordered_map<FunctorBase *, std::string> f_to_name;
  const auto &keys = vars.GetKeys();
  for (const auto &k : keys) {
    if (IsFunctor(k)) {
      const auto &entry = vars.GetValue<EnvFunctorEntry>(k);
      if (entry.IsCreated()) {
        f_to_name[entry.GetFunctor().get()] = k;
      }
    }
  }
  return f_to_name;
}

template <typename ContainerStrT>
inline
void env::MoveVarsTo(const ContainerStrT &vars, env &e) {
  for (const auto &varname : vars) { MoveVarTo(varname, e); }
}

inline
void env::MoveVarTo(const std::string &varname, env &e) {
  if (OK_UNLIKELY(IsEnvVar(varname))) { return; }
  vars.MoveKeyTo(varname, e.vars);
}

template <typename ContainerStrT>
inline
env env::Splice(const ContainerStrT &vars) {
  env e = EmptyClone();
  MoveVarsTo(vars, e);
  return e;
}

} /* namespace fli */
} /* namespace ok */

#endif /* OK_FLI_RUNTIME_ENV_INL_HPP_ */
