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
#ifndef OK_SERIALIZATIONUTILS_DYNAMICPROTO_DYNAMICPROTOUTILS_NOPB_HPP_
#define OK_SERIALIZATIONUTILS_DYNAMICPROTO_DYNAMICPROTOUTILS_NOPB_HPP_

#ifndef OK_SERIALIZATIONUTILS_DYNAMICPROTO_DYNAMICPROTOUTILS_H_
#error "Include DynamicProtoUtils.hpp instead"
#endif

#if OK_ENABLE_PROTOBUF
#error "Include this file only if protobuf is not available"
#endif

namespace ok {

template <typename T>
inline
bool DynHasValue(const ok_msg::DynamicProto &dyn) { return false; }

template <typename T>
inline
T DynGetValue(const ok_msg::DynamicProto &dyn) {
  OKASSERT_FATAL(false, "OK_ENABLE_PROTOBUF required");
  return detail::DynZeroValue<T>();
}

template <typename T>
inline
IArray<T> DynGetArray(const ok_msg::DynamicProto &dyn) {
  return IArray<T>();
}

inline
bool DynHasKey(const ok_msg::DynamicProto &dyn, const std::string &key) {
  return false;
}

template <typename T>
inline
T DynGetValueForKey(
    const ok_msg::DynamicProto &dyn,
    const std::string &key) {

  OKASSERT_FATAL(false, "OK_ENABLE_PROTOBUF required");
  return detail::DynZeroValue<T>();
}

template <typename T>
inline
IArray<T> DynGetArrayForKey(
    const ok_msg::DynamicProto &dyn,
    const std::string &key) {

  OKASSERT_FATAL(false, "OK_ENABLE_PROTOBUF required");
  return IArray<T>();
}

inline
const ok_msg::DynamicProto *DynGetDynForKey(
    const ok_msg::DynamicProto &dyn,
    const std::string &key) {

  OKASSERT_FATAL(false, "OK_ENABLE_PROTOBUF required");
  return nullptr;
}

template <typename T>
inline
void DynSetValue(ok_msg::DynamicProto &dyn, const T &v) { }

template <typename T>
inline
void DynSetArray(ok_msg::DynamicProto &dyn, const IArray<T> &arr) { }

template <typename T>
inline
void DynSetValueForKey(
    ok_msg::DynamicProto &dyn,
    const std::string &key,
    const T &val) { }

template <typename T>
inline
void DynSetArrayForKey(
    ok_msg::DynamicProto &dyn,
    const std::string &key,
    const IArray<T> &arr) { }

inline
void DynSetDynForKey(
    ok_msg::DynamicProto &dyn,
    const std::string &key,
    ok_msg::DynamicProto *d) { }

inline
void DynSetDynForKey(
    ok_msg::DynamicProto &dyn,
    const std::string &key,
    ok_msg::DynamicProto &&d) { }

inline
void DynSetDynForKey(
    ok_msg::DynamicProto &dyn,
    const std::string &key,
    const ok_msg::DynamicProto &d) { }

} /* namespace ok */

#endif /* OK_SERIALIZATIONUTILS_DYNAMICPROTO_DYNAMICPROTOUTILS_NOPB_HPP_ */
