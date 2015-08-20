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

#include "ok/fli/Runtime/EnvFunctorEntryWrapper.hpp"

#include <sstream>

#include "ok/fli/Runtime/env.hpp"
#include "ok/fli/Utils/Serialization/PBEncodingUtils.hpp"

namespace ok { namespace fli {

std::string EnvFunctorEntryWrapper::ToString() const {
  std::stringstream ss;
  ss << "EnvFunctorEntryWrapper(" << std::endl;

  ss << "  Functor: ";
  if (IsCreated()) { ss << func_->ToString(); } else { ss << "(not created)"; }
  ss << std::endl;

  if (!attrs_.IsEmpty()) {
    ss << "  Attributes: " << attrs_.ToString() << std::endl;
  }

  ss << ")" << std::endl;
  return ss.str();
}

bool EnvFunctorEntryWrapper::CreateFunctor(env &env) {
  if (OK_UNLIKELY(IsCreated())) { return true; }

  #if OK_ENABLE_PROTOBUF
  if (attrs_.HasAttr<PBFuncAttr>()) {
    ok_msg::Func &func_spec = attrs_.GetAttrRef<PBFuncAttr>();
    if (func_spec.has_alias()) {
      return CreateFunctor(env, func_spec.alias());
    }
  }
  #endif

  OKLOG_ERROR(
    "Don't know how to create functor from given attributes; " <<
    "ignoring: " << attrs_.ToString());
  return false;
}

bool EnvFunctorEntryWrapper::CreateFunctor(
    env &env,
    const std::string &alias) {

  auto f = NullTUPtr<FunctorBase>();

  // Create the functor
  {
    f = env.GetRegistry().CreateFunctor(alias);
    if (!OKASSERT_CHECK(f.get(), "Failed to create a " + alias)) {
      return false;
    }

    bool decoded = f->FromEncoded(env, attrs_);
    if (!OKASSERT_CHECK(
           decoded,
           "Failed to decode functor " + alias)) {
      return false;
    }
  }

  SetFunctor(std::move(f));

  return true;
}

void EnvFunctorEntryWrapper::SetFunctor(FunctorBase::Ptr &&f) {
  func_ = std::move(f);

  // Cleanup: remove any serialization data so that it won't be
  // re-serialized
  attrs_.ClearAttr<PBFuncAttr>();
}

bool EnvFunctorEntryWrapper::EncodeFunctor(env &env) {
  if (!IsCreated()) {
    bool have_encoding = false;
    #if OK_ENABLE_PROTOBUF
    have_encoding |=
        env.HasEncoding<PBFuncAttr>() && attrs_.HasAttr<PBFuncAttr>();
    #endif

    return have_encoding;
  }

  if (!OKASSERT_CHECK(func_->ToEncoded(env, attrs_), "Encoding error")) {
    return false;
  }

  // Special handling for stateless Functors.  Helpers only return
  // false on error.
  bool stateless_success = false;
  #if OK_ENABLE_PROTOBUF
  stateless_success |= EncodeStatelessFunctor(env, attrs_, func_.get());
  #endif

  return stateless_success;
}

#if OK_ENABLE_PROTOBUF

bool EnvFunctorEntryWrapper::FromPBMessage(ok_msg::EnvFunctorEntryData &m) {

  bool is_emtpy = !IsCreated();
  if (!OKASSERT_CHECK(
        is_emtpy,
        "Programming error: this entry has already been initialized")) {
    return false;
  }

  if (m.has_func()) {
    attrs_.SetAttr<PBFuncAttr>(ok_msg::Func());
    attrs_.GetAttrRef<PBFuncAttr>().Swap(m.mutable_func());
  }

  RegisterFuncAttrs(attrs_);
  if (m.has_attrs()) {
    bool attrs_decoded = attrs_.FromPBMessage(*m.mutable_attrs());
    if (!OKASSERT_CHECK(attrs_decoded, "Failed to decode attributes")) {
      return false;
    }
  }

  return true;
}

bool EnvFunctorEntryWrapper::ToPBMessage(ok_msg::EnvFunctorEntryData &m) {

  if (attrs_.HasAttr<PBFuncAttr>()) {

    // == Encode from Attrs ==================================================
    m.mutable_func()->Swap(&attrs_.GetAttrRef<PBFuncAttr>());
    attrs_.ClearAttr<PBFuncAttr>();

  } else {

    // == We didn't / can't encode :( ========================================
    OKLOG_ERROR(
      "Don't have encoding for this wrapped functor: " << ToString());
    return false;

  }

  // Encode the remaining attrs
  bool attrs_encoded = attrs_.ToPBMessage(*m.mutable_attrs());
  if (!OKASSERT_CHECK(
         attrs_encoded,
         "Failed to encode attributes: " + attrs_.ToString())) {
    return false;
  }

  return true;
}

#endif

} /* namespace fli */
} /* namespace ok */
