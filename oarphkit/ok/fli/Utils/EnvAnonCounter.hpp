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
#ifndef OK_FLI_UTILS_ENVANONCOUNTER_HPP_
#define OK_FLI_UTILS_ENVANONCOUNTER_HPP_

#include <atomic>

#include "ok/SerializationUtils/DynamicProto/DynamicProtoUtils.hpp"
#include "ok/SerializationUtils/IPBSerializable.hpp"

namespace ok { namespace fli {

/**
 * A counter to help env generate distinct variable names.
 *
 * We define this as a separate struct because:
 *  * atomic isn't default-move-constructible
 *  * we need non-standard control over serialization
 */
struct EnvAnonCounter final : public IPBSerializable<ok_msg::DynamicProto> {

  std::atomic_uint_fast64_t val;

  EnvAnonCounter() : val(0) { }
  ~EnvAnonCounter() { }

  // For our uses (serialization), copying the counter value is safe
  EnvAnonCounter(EnvAnonCounter &&other) { val = other.val.load(); }
  EnvAnonCounter& operator=(EnvAnonCounter &&other) {
    val = other.val.load();
    return *this;
  }

  inline std::string ToString() const {
    return "EnvAnonCounter(val:" + std::to_string(val.load()) + ")";
  }

  #if OK_ENABLE_PROTOBUF
  bool FromPBMessage(ok_msg::DynamicProto &m) override {
    if (!OKASSERT_CHECK(DynHasValue<uint64_t>(m), "Message missing value")) {
      return false;
    }
    val = DynGetValue<uint64_t>(m);
    return true;
  }

  bool ToPBMessage(ok_msg::DynamicProto &m) override {
    DynSetValue<uint64_t>(m, val);
    return true;
  }
  #endif

};

} /* namespace fli */
} /* namespace ok */

#endif /* OK_FLI_UTILS_ENVANONCOUNTER_HPP_ */
