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
#ifndef OK_STRINGIFY_INL_HPP_
#define OK_STRINGIFY_INL_HPP_

#ifndef OK_STRINGIFY_HPP_
#error "Include Stringify.hpp instead"
#endif

#include <iomanip>
#include <list>
#include <sstream>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "ok/BranchPredict.hpp"
#include "ok/Common.hpp"

namespace ok { namespace detail {

template <typename T>
using is_vector =
    std::is_same<T,
        std::vector<typename T::value_type, typename T::allocator_type>>;

template <typename T>
using is_list =
    std::is_same<T,
        std::list<typename T::value_type, typename T::allocator_type>>;

template <typename T>
using is_unordered_map =
    std::is_same<T,
        std::unordered_map<typename T::key_type, typename T::mapped_type>>;

// TODO: specialization for atomic?

template <typename T>
using pointed_val = typename std::remove_reference<decltype(*std::declval<T>())>::type;

template <typename C>
struct has_to_string {
  template <typename T>
  static constexpr auto check(T*)
    -> typename
      std::is_same<
        decltype(std::declval<T>().ToString()),
        std::string
      >::type { return {}; }

  template <typename>
  static constexpr std::false_type check(...) { return {}; }

    // NB: we return {}; above to work around a -Wundefined-internal bug

  typedef decltype(check<C>(0)) type;

public:
  static constexpr bool value = type::value;
};

template <typename C>
struct has_begin {
  template <typename T>
  static constexpr auto check(T*)
    -> typename
      std::is_same<
        decltype(std::declval<T>().begin(), void(), std::string{}),
        std::string
      >::type { return {}; }

  template <typename>
  static constexpr std::false_type check(...) { return {}; }

    // NB: we return {}; above to work around a -Wundefined-internal bug

  typedef decltype(check<C>(0)) type;

public:
    static constexpr bool value = type::value;
};

// Try to use existing operator<< for T via SFINAE
template <typename T>
inline
auto PutString(std::stringstream &ss, const T &v, int32_t max_len, int)
  -> decltype(ss << v, void(), std::string{}) {

  /**
   * Notes:
   *  * C++11 SFINAE works as follows: the decltype above will fail if
   *      operator<< does not exist for T, and result in a return type of
   *      std::string otherwise.
   *  * The final bool argument is a 'tag' to make SFINAE prefer this
   *      function over the default below (for opaque values).
   *  * Based upon http://coliru.stacked-crooked.com/a/8d01b86ce9a24e25
   */

  // Stringify the thing!
  ss << v;

  // Truncate if necessary
  auto s = ss.str();
  if (max_len >= 0 && s.size() > max_len) {
    return s.substr(0, max_len) + strify::kEllipses;
  }
  return s;
}

// Try to use ToString() via SFINAE
template <typename T,
          typename std::enable_if<has_to_string<T>::value>::type* = nullptr>
inline
auto PutString(std::stringstream &ss, const T &v, int32_t max_len, bool)
  -> decltype(ss << v.ToString(), void(), std::string{}) {

  ss << v.ToString();

  // Truncate if necessary
  auto s = ss.str();
  if (max_len >= 0 && s.size() > max_len) {
    return s.substr(0, max_len) + strify::kEllipses;
  }
  return s;
}

// Try to use ToString() from a pointer via SFINAE
template <typename T,
          typename std::enable_if<
                      has_to_string<pointed_val<T>>::value>::type* = nullptr>
inline
auto PutString(std::stringstream &ss, const T &v, int32_t max_len, bool)
  -> decltype(ss << v->ToString(), void(), std::string{}) {

  if (!v) { return "nullptr"; }

  ss << v->ToString();

  // Truncate if necessary
  auto s = ss.str();
  if (max_len >= 0 && s.size() > max_len) {
    return s.substr(0, max_len) + strify::kEllipses;
  }
  return s;
}

template <typename VectorT,
          typename std::enable_if<is_vector<VectorT>::value>::type* = nullptr>
inline
auto PutString(std::stringstream &ss, const VectorT &v, int32_t max_len, bool)
  -> decltype(ss << Stringify(v.at(0)), void(), std::string{}) {

  if (v.empty()) { return "vector(empty)"; }

  ss << "vector(sz:" << v.size() << "; " << Stringify(v[0]);
  for (size_t i = 1; i < v.size(); ++i) {
    ss << ", " << Stringify(v[i]);

    // Break a little early if we've already written more than needed
    if (ss.tellp() > max_len) { break; }
  }
  ss << ")";

  // Truncate if necessary
  auto s = ss.str();
  if (max_len >= 0 && s.size() > max_len) {
    return s.substr(0, max_len) + "..)"; // abbreviated truncation for vector
  }
  return s;
}

template <typename ListT,
          typename std::enable_if<is_list<ListT>::value>::type* = nullptr>
inline
auto PutString(std::stringstream &ss, const ListT &lst, int32_t max_len, bool)
  -> decltype(ss << Stringify(lst.front()), void(), std::string{}) {

  auto it = lst.begin();
  if (it == lst.end()) { return "list(empty)"; }

  ss << "list(" << Stringify(*it);
  ++it;
  for ( ; it != lst.end(); ++it) {
    ss << ", " << Stringify(*it);

    // Break a little early if we've already written more than needed
    if (ss.tellp() > max_len) { break; }
  }
  ss << ")";

  // Truncate if necessary
  auto s = ss.str();
  if (max_len >= 0 && s.size() > max_len) {
    return s.substr(0, max_len) + "..)"; // abbreviated truncation for list
  }
  return s;
}

template <typename MapT,
          class = typename std::enable_if<is_unordered_map<MapT>::value>::type>
inline
auto PutString(std::stringstream &ss, const MapT &m, int32_t max_len, bool)
  -> std::string {

  if (m.empty()) { return "unordered_map(empty)"; }

  ss << "unordered_map(sz:" << m.size() << "; ";
  size_t i = 0;
  for (const auto &entry : m) {
    ss << Stringify(entry.first) << ":" << Stringify(entry.second);
    ++i;
    if (i != m.size()) { ss << ";"; }

    // Break a little early if we've already written more than needed
    if (ss.tellp() > max_len) { break; }
  }
  ss << ")";

  // Truncate if necessary
  auto s = ss.str();
  if (max_len >= 0 && s.size() > max_len) {
    return s.substr(0, max_len) + "..)"; // abbreviated truncation for map
  }
  return s;
}

// TODO want one for unordered_set, or arbitrary sequences...
//// Try to use begin() end() range via SFINAE
//template <typename T,
//          typename std::enable_if<has_begin<T>::value>::type* = nullptr>
//inline
//auto PutString(std::stringstream &ss, const T &seq, int32_t max_len, std::nullptr_t)
//  -> decltype(ss << Stringify(*seq.begin()), void(), std::string{}) {
//
//  if (seq.begin() == seq.end()) {
//    return "sequence(emtpy)";
//  }
//  ss << "sequence(";
//  size_t i = 0;
//  for (const auto &val : seq) {
//    if (OK_LIKELY(i != 0)) { ss << ", "; }
//    ss << Stringify(val);
//    ++i;
//
//    // Break a little early if we've already written more than needed
//    if (ss.tellp() > max_len) { break; }
//  }
//  ss << ")";
//
//  // Truncate if necessary
//  auto s = ss.str();
//  if (max_len >= 0 && s.size() > max_len) {
//    return s.substr(0, max_len) + strify::kEllipses;
//  }
//  return s;
//}

// Fallback for T without operator<<
template <typename T>
inline
auto PutString(std::stringstream &ss, const T &v, int32_t max_len, float)
  -> std::string {

  return ::ok::strify::kOpaque;
}

} /* namespace detail */


////
//// Base Impl
////

template <typename T>
inline
std::string Stringify(const T &v, int32_t max_len) {
  std::stringstream ss;
  return detail::PutString(ss, v, max_len, true);
                                // bool tag is for overload preference
}

// Try to return a std::string version of `v` or strify::kOpaque upon failure.
template <typename T>
inline
std::string Stringify(const T &v) {
  return Stringify(v, strify::kDefaultMaxLen);
}

template <typename T>
inline
std::string StringifyNoLimit(const T &v) {
  return Stringify(v, -1);
}

////
//// Specializations
////

template <>
inline
std::string Stringify<byte>(const byte &v) {
  std::stringstream ss;
  ss << std::hex << std::setfill('0') << std::setw(1) << int(v);
  return ss.str();
}

} /* namespace ok */

#endif /* OK_STRINGIFY_INL_HPP_ */
