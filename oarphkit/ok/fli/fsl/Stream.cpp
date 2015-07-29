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
#include "ok/fli/fsl/Stream.hpp"

#include <sstream>

#include "ok/fli/Runtime/env.hpp"
#include "ok/fli/Utils/Serialization/PBEncodingUtils.hpp"

namespace ok { namespace fli { namespace fsl {

void Stream::Call() {
  if (!OKASSERT_CHECK(src_ && snk_, "Invalid Stream: " + ToString())) {
    return;
  }

  rt_datumptr temp = src_->Call(rt_datumptr());
  while (temp.value) {
    snk_->Call(std::move(temp));
    temp = src_->Call(rt_datumptr());
  }
}

bool Stream::SetSource(FunctorBase::Ptr &&f) {
  if (!OKASSERT_CHECK(f, "Null Source")) { return false; }

  bool src_ok =
      OK_FLI_CT_CHECK(
        f->InType(),
        typeid(Void),
        "Cannot accept non-source as source: " +
          f->ToString());
  if (snk_) {
    src_ok &=
        OK_FLI_CT_CHECK(
            f->OutType(),
            snk_->InType(),
            "Given source incompatible with current sink. " +
              "Sink: " + snk_->ToString() + " " +
              "Given Source: " + f->ToString());
  }

  if (src_ok) {
    src_ = std::move(f);
    return true;
  }

  return false;
}

bool Stream::SetSink(FunctorBase::Ptr &&f) {
  if (!OKASSERT_CHECK(f, "Null Sink")) { return false; }

  bool snk_ok =
      OK_FLI_CT_CHECK(
        f->OutType(),
        typeid(Void),
        "Cannot accept non-sink as sink: " +
          f->ToString());
  if (src_) {
    snk_ok &=
        OK_FLI_CT_CHECK(
            src_->OutType(),
            f->InType(),
            "Given sink incompatible with current source. " +
              "Source: " + src_->ToString() + " " +
              "Given Sink: " + f->ToString());
  }

  if (snk_ok) {
    snk_ = std::move(f);
    return true;
  }

  return false;
}

std::string Stream::ToString() const {
  std::stringstream ss;
  ss << "Stream(" << GetSignature() << ":";
  ss << " src=" << (src_ ? src_->ToString() : "") << ";";
  ss << " snk=" << (snk_ ? snk_->ToString() : "");
  ss << ")";
  return ss.str();
}

bool Stream::FromEncoded(env &env, SVStruct &m) {

  bool decoded = false;

  // = BEGIN PB ==============================================================
  #if OK_ENABLE_PROTOBUF
  decoded |=
    DecodeFromPBFunc(
      env,
      m,
      ok_msg::StreamInit::stream,
      [&] (ok_msg::Func &spec) {
        auto stream_spec = spec.MutableExtension(ok_msg::StreamInit::stream);

        auto *src_spec = stream_spec->mutable_src();
        bool src_success =
            SetSource(env.CreateFunctor(FuncAttrsFromPB(*src_spec)));
        if (!OKASSERT_CHECK(
              src_success,
              "Could not create or set source func: " +
                  PBFactory::AsTextFormatString(*src_spec))) {

          return false;
        }

        auto *snk_spec = stream_spec->mutable_snk();
        bool snk_success =
            SetSink(env.CreateFunctor(FuncAttrsFromPB(*snk_spec)));
        if (!OKASSERT_CHECK(
              snk_success,
              "Could not create or set sink func: " +
                  PBFactory::AsTextFormatString(*snk_spec))) {

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

bool Stream::ToEncoded(env &env, SVStruct &m) {

  bool encoded = false;

  // = BEGIN PB ==============================================================
  #if OK_ENABLE_PROTOBUF
  encoded |=
    EncodeToPBFunc(
      env,
      m,
      RegistryAlias(),
      [&] (ok_msg::Func &spec) {
        auto stream_spec =
            spec.MutableExtension(
                ok_msg::StreamInit::stream);

        // Encode functors as references to env variables
        auto f_to_name = env.GetFunctorToName();

        if (src_) {
          auto f_key = src_.get();
          if (!OKASSERT_CHECK(
                 f_to_name.find(f_key) != f_to_name.end(),
                 "Can't find source functor")) {
            return false;
          }

          stream_spec->
            mutable_src()->
            set_name(f_to_name.at(f_key));
        }

        if (snk_) {
          auto f_key = snk_.get();
          if (!OKASSERT_CHECK(
                 f_to_name.find(f_key) != f_to_name.end(),
                 "Can't find sink functor")) {
            return false;
          }

          stream_spec->
            mutable_snk()->
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

} /* namespace fsl */
} /* namespace fli */
} /* namespace ok */
