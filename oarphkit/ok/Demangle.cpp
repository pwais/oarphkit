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

#include "ok/Demangle.hpp"

// Note that modern clang also supports this feature
#if __GNUC__
#include <cxxabi.h>
#include <stdlib.h>

namespace ok {

// Try to demangle a name from a type object
template <typename TypeInfoT>
inline
std::string DemangledNameImpl(const TypeInfoT &t) {
  const std::string mangled_name = std::string(t.name());

  int demangle_status;
  char *demangled_name =
      abi::__cxa_demangle(
          t.name(),
          nullptr, // Force a heap-allocated buffer
          nullptr,
          &demangle_status);
  const std::string name =
      (demangled_name != nullptr) ?
          std::string(demangled_name) : mangled_name;

  free(demangled_name);
  return name;
}

} /* namespace ok */

#else

namespace ok {

template <typename TypeInfoT>
inline
std::string DemangledNameImpl(const TypeInfoT &t) {
  return std::string(t.name());
}

} /* namespace ok */

#endif

namespace ok {

std::string DemangledName(const std::type_index &t) {
  return DemangledNameImpl(t);
}

std::string DemangledName(const std::type_info &t) {
  return DemangledNameImpl(t);
}

} /* namespace ok */



