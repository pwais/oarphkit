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

#include "ok/fli/Runtime/Registry.hpp"

#include <algorithm>
#include <sstream>
#include <vector>

namespace ok { namespace fli {

bool Registry::UnionWith(const Registry &other) {
  bool all_accepted = true;
  for (const auto &entry : other.alias_to_factory_) {
    const std::string &alias = entry.first;
    bool alias_taken =
        alias_to_factory_.find(alias) != alias_to_factory_.end();
    if (!OKASSERT_CHECK(
         !alias_taken,
         "Warning: Registry " + name_ + " has existing entry for " +
         alias + "; not adding from " + other.GetName())) {

      all_accepted = false;
      continue;
    }

    alias_to_factory_[alias] = entry.second;
  }

  return all_accepted;
}

std::string Registry::GetAllAliases(const std::string &delimiter) const {
  std::vector<std::string> sorted_aliases;
  sorted_aliases.reserve(alias_to_factory_.size());
  for (const auto &entry : alias_to_factory_) {
    sorted_aliases.push_back(entry.first);
  }
  std::sort(sorted_aliases.begin(), sorted_aliases.end());

  std::stringstream ss;
  if (sorted_aliases.empty()) {
    ss << "(empty)";
  } else {
    ss << sorted_aliases[0];
    for (int i = 1; i < sorted_aliases.size(); ++i) {
      ss << delimiter << sorted_aliases[i];
    }
  }
  return ss.str();
}

std::string Registry::ToString() const {
  std::stringstream ss;
  ss << "Registry(\n\n";

  ss << "  name: " << name_ << "\n";

  ss << "  aliases:\n";
  ss << "  " << GetAllAliases("\n  ") << "\n";

  ss << ")";
  return ss.str();
}

} /* namespace fli */
} /* namespace ok */
