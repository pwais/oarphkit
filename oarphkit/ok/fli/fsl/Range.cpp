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

#include "ok/fli/fsl/Range.hpp"

#include <sstream>

#include "ok/Config.hpp"
#include "ok/fli/Utils/Serialization/PBEncodingUtils.hpp"

namespace ok { namespace fli { namespace fsl {

std::string Range::ToString() const {
  std::stringstream ss;
  ss << "Range(" <<
          GetSignature() << ";" <<
          "current:" << current_ << ";" <<
          "step:" << step_ << ";" <<
          "limit:" << limit_ << ")";
  return ss.str();
}

bool Range::FromEncoded(env &env, SVStruct &m) {
  bool decoded = false;

  // = BEGIN PB ==============================================================
  #if OK_ENABLE_PROTOBUF
  decoded |=
    DecodeFromPBFunc(
      env,
      m,
      ok_msg::RangeInit::range,
      [&] (ok_msg::Func &spec) {
        auto range_spec = spec.MutableExtension(ok_msg::RangeInit::range);
        current_ = range_spec->start();
        step_ = range_spec->step();
        limit_ = range_spec->limit();
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

bool Range::ToEncoded(env &env, SVStruct &m) {
  bool encoded = false;

  // = BEGIN PB ==============================================================
  #if OK_ENABLE_PROTOBUF
  encoded |=
    EncodeToPBFunc(
      env,
      m,
      RegistryAlias(),
      [&] (ok_msg::Func &spec) {
        auto range_spec = spec.MutableExtension(ok_msg::RangeInit::range);
        range_spec->set_start(current_);
        range_spec->set_step(step_);
        range_spec->set_limit(limit_);
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
