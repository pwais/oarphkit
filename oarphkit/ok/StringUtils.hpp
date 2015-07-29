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
#ifndef OK_STRINGUTILS_HPP_
#define OK_STRINGUTILS_HPP_

#include <string>

#include "ok/BranchPredict.hpp"

namespace ok {

// Return true only if `prefix` is a prefix of `base`
template <typename SeqBaseT, typename SeqPreT>
bool StartsWith(const SeqBaseT &base, const SeqPreT &prefix) {
  if (OK_UNLIKELY(prefix.size() > base.size())) { return false; }
  for (size_t i = 0; i < prefix.size(); ++i) {
    if (base[i] != prefix[i]) { return false; }
  }
  return true;
}

// For forcing c-str coercion
inline
bool SStartsWith(const std::string &base, const std::string &prefix) {
  return StartsWith(base, prefix);
}

template <typename SeqBaseT, typename SeqSufT>
bool EndsWith(const SeqBaseT &base, const SeqSufT &suffix) {
  if (OK_UNLIKELY(suffix.size() > base.size())) { return false; }
  const size_t offset = base.size() - suffix.size();
  for (size_t i = 0; i < suffix.size(); ++i) {
    if (base[i + offset] != suffix[i]) { return false; }
  }
  return true;
}

// For forcing c-str coercion
inline
bool SEndsWith(const std::string &base, const std::string &suffix) {
  return EndsWith(base, suffix);
}

inline
void SReplaceAllInPlace(
    std::string &s,
    const std::string &target,
    const std::string &replace) {

  size_t pos = s.find(target, 0);
  while (pos != std::string::npos && pos < s.size()) {
    s.replace(pos, target.size(), replace);
    pos += replace.size();
    pos = s.find(target, pos);
  }
}

} /* namespace ok */

#endif /* OK_STRINGUTILS_HPP_ */
