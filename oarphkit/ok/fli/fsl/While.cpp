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
#include "ok/fli/fsl/While.hpp"

#include <sstream>

#include "ok/fli/Runtime/env.hpp"
#include "ok/fli/Utils/Serialization/PBEncodingUtils.hpp"
#include "ok/SerializationUtils/DynamicProto/DynamicProtoUtils.hpp"

namespace ok { namespace fli { namespace fsl {

void While::Call() {
  const bool valid = condition_ && body_;
  OKASSERT_FATAL(valid, "Invalid While: " + ToString());
  if (OK_UNLIKELY(!valid)) { return; }

  while (condition_->Call(rt_datumptr()).GetRef<bool>()) {
    body_->Call(rt_datumptr());
  }
}

bool While::SetCondition(FunctorBase::Ptr &&f) {
  if (!OKASSERT_CHECK(f, "Null Condition")) { return false; }

  bool condition_is_src_bool =
      OK_FLI_CT_CHECK(
        f->InType(),
        typeid(Void),
        "Cannot accept condition function has non-void input type: " +
          f->ToString())

              &&

      OK_FLI_CT_CHECK(
        f->OutType(),
        typeid(bool),
        "Cannot accept condition function that does not generate bool: " +
          f->ToString());

  if (condition_is_src_bool) {
    condition_ = std::move(f);
    return true;
  }

  return false;
}

bool While::SetBody(FunctorBase::Ptr &&f) {
  if (!OKASSERT_CHECK(f, "Null Body")) { return false; }

  bool body_is_subroutineable =
      OK_FLI_CT_CHECK(
        f->InType(),
        typeid(Void),
        "Cannot accept body function has non-void input type: " +
          f->ToString());
  if (body_is_subroutineable) {
    body_ = std::move(f);
    return true;
  }

  return false;
}

std::string While::ToString() const {
  std::stringstream ss;
  ss << "While(" << GetSignature() << ":";
  ss << "  cond=" << (condition_ ? condition_->ToString() : "") << ";";
  ss << "  body=" << (body_ ? body_->ToString() : "");
  ss << ")";
  return ss.str();
}

bool While::FromEncoded(env &env, SVStruct &m) {

  bool decoded = false;

  // = BEGIN PB ==============================================================
  #if OK_ENABLE_PROTOBUF
  decoded |=
    DecodeFromPBFunc(
      env,
      m,
      ok_msg::WhileInit::while_,
      [&] (ok_msg::Func &spec) {
        auto while_spec =
            spec.MutableExtension(
                ok_msg::WhileInit::while_);

        auto *cond_spec = while_spec->mutable_condition();
        bool cond_success =
            SetCondition(env.CreateFunctor(FuncAttrsFromPB(*cond_spec)));
        if (!OKASSERT_CHECK(
              cond_success,
              "Could not create or set condition func")) {
          return false;
        }

        auto *body_spec = while_spec->mutable_body();
        bool body_success =
            SetBody(env.CreateFunctor(FuncAttrsFromPB(*body_spec)));
        if (!OKASSERT_CHECK(
              body_success,
              "Could not create or set body func")) {
          return false;
        }

        return true;
      });
  #endif
  // = END PB ================================================================

  #if OK_ENABLE_CAPNP
  // TODO (CAPNP)
  #endif

  return
    OKASSERT_CHECK(decoded, "Don't know how to decode from " + m.ToString());
}

bool While::ToEncoded(env &env, SVStruct &m) {

  bool encoded = false;

  // = BEGIN PB ==============================================================
  #if OK_ENABLE_PROTOBUF
  encoded |=
    EncodeToPBFunc(
      env,
      m,
      RegistryAlias(),
      [&] (ok_msg::Func &spec) {
        auto while_spec = spec.MutableExtension(ok_msg::WhileInit::while_);

        // Encode functors as references to env variables
        auto f_to_name = env.GetFunctorToName();

        if (condition_) {
          auto f_key = condition_.get();
          if (!OKASSERT_CHECK(
                 f_to_name.find(f_key) != f_to_name.end(),
                 "Can't find condition functor")) {
            return false;
          }

          while_spec->
            mutable_condition()->
            set_name(f_to_name.at(f_key));
        }

        if (body_) {
          auto f_key = body_.get();
          if (!OKASSERT_CHECK(
                 f_to_name.find(f_key) != f_to_name.end(),
                 "Can't find body functor")) {
            return false;
          }

          while_spec->
            mutable_body()->
            set_name(f_to_name.at(f_key));
        }

        return true;
      });
  #endif
  // = END PB ================================================================

  #if OK_ENABLE_CAPNP
  // TODO (CAPNP)
  #endif

  return
    OKASSERT_CHECK(encoded, "Don't know how to encode to " + m.ToString());
}

std::string While::ForN::ToString() const {
  std::stringstream ss;
  ss << "ForN(i:" << i_ << ";n:" << n_ << ";" << GetSignature() << ")";
  return ss.str();
}

bool While::ForN::FromEncoded(env &env, SVStruct &m) {

  bool decoded = false;

  // = BEGIN PB ==============================================================
  #if OK_ENABLE_PROTOBUF
  decoded |=
    DecodeFromPBFuncDyn(
      env,
      m,
      [&] (ok_msg::DynamicProto &dyn) {
        i_ = DynGetValueForKeyOrDefault<uint64_t>(dyn, "i", 0);
        n_ = DynGetValueForKeyOrDefault<uint64_t>(dyn, "n", 0);
        return true;
      });
  #endif
  // = END PB ================================================================

  #if OK_ENABLE_CAPNP
  // TODO (CAPNP)
  #endif

  return
    OKASSERT_CHECK(decoded, "Don't know how to decode from " + m.ToString());
}

bool While::ForN::ToEncoded(env &env, SVStruct &m) {

  bool encoded = false;

  // = BEGIN PB ==============================================================
  #if OK_ENABLE_PROTOBUF
  encoded |=
    EncodeToPBFuncDyn(
      env,
      m,
      RegistryAlias(),
      [&] (ok_msg::DynamicProto &dyn) {
        DynSetValueForKey<uint64_t>(dyn, "i", i_);
        DynSetValueForKey<uint64_t>(dyn, "n", n_);
        return true;
      });
  #endif
  // = END PB ================================================================

  #if OK_ENABLE_CAPNP
  // TODO (CAPNP)
  #endif

  return
    OKASSERT_CHECK(encoded, "Don't know how to encode to " + m.ToString());
}

} /* namespace fsl */
} /* namespace fli */
} /* namespace ok */
