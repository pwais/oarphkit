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

#include "ok/SV/SVStruct.hpp"

#include <sstream>

#include "ok/Stringify.hpp"

namespace ok {

std::string SVStruct::ToString() const {
  std::stringstream ss;
  ss << "\n--- --- SVStruct Begin --- ---\n\n";
  for (const auto &entry : entrymap_) {
    ss << entry.first << " (" << Stringify((void *)entry.second->Key()) << ") -> " <<
        entry.second->ToString()
        << "\n\n";
  }
  ss << "--- --- SVStruct End --- ---\n\n";
  return ss.str();
}

detail::SVBase::SVRefMap SVStruct::GetSVRefMap() const {
  detail::SVBase::SVRefMap ref_map;
  ref_map.reserve(entrymap_.size());
  for (const auto &entry : entrymap_) {
    ref_map.push_back(
      std::make_pair(
        nullptr,
        entry.second.get()));
  }
  return ref_map;
}

} /* namespace ok */




