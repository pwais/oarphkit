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
#ifndef OK_STRINGIFY_HPP_
#define OK_STRINGIFY_HPP_

#include <string>

namespace ok {

namespace strify {

static const int32_t kDefaultMaxLen = 120;

static const std::string kOpaque = "[Opaque to ok::Stringify]";

static const std::string kEllipses = "...";

} /* namespace strify */

/**
 * Try to return a std::string version of `v` or kOpaque upon failure;
 * truncate the returned string to at most `max_len` characters
 * followed by "..." (or all the characters if max_len is negative).
 */
template <typename T>
std::string Stringify(const T &v, int32_t max_len);

// Try to return a std::string version of `v` or strify::kOpaque upon failure.
template <typename T>
std::string Stringify(const T &v);

template <typename T>
std::string StringifyNoLimit(const T &v); // Equivalent to -1 for max_len

  /**
   * NB: See StringifyTest for deets, but currently we support
   * have non-kOpaque output for:
   *
   *  * bytes in hex
   *  * T with ostream<<
   *  * T with ToString()
   *  * T * with ToString() (including {unique,shared}_ptr)
   *  * std::vector<T> (with any of the T above)
   *  * std::list<T> (with any of the T above)
   *  * std::unordered_map<K, V> (with any of the T={K,V} above)
   */

} /* namespace ok */

#include "ok/Stringify-inl.hpp"

#endif /* OK_STRINGIFY_HPP_ */
