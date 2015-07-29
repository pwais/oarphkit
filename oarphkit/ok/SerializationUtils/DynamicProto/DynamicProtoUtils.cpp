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

#include "ok/SerializationUtils/DynamicProto/DynamicProtoUtils.hpp"

#include <sstream>

#include "ok/SerializationUtils/PBUtils.hpp"

namespace ok {

std::string DynToString(const ok_msg::DynamicProto &p) {

  std::stringstream ss;
  ss << "DynamicProto(";

  #if OK_ENABLE_PROTOBUF
    const std::string s = PBFactory::AsTextFormatString(p, true);
  #else
    const std::string s = "";
  #endif

  if (OK_LIKELY(!s.empty())) {
    ss << s << "\n";
  }
  ss << ")";
  return ss.str();
}

} /* namespace ok */
