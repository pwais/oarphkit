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
#ifndef OK_SERIALIZATIONUTILS_DYNAMICPROTO_DYNAMICPROTOUTILS_H_
#define OK_SERIALIZATIONUTILS_DYNAMICPROTO_DYNAMICPROTOUTILS_H_

#include <string>

#include "ok/Config.hpp"
#include "ok/IArray.hpp"

#if OK_ENABLE_PROTOBUF
#include "ok_msg/DynamicProto.pb.h"
#else
// A stub to appease the compiler; users won't use this though
namespace ok_msg { struct DynamicProto { }; }
#endif

namespace ok {

// == Extract Data ===========================================================

/*
 * Nota Bene: if you elide asserts and have data with missing fields, you
 * may encounter correct but very unexpected behavior from `DynGetValue`
 * (and friends), which will return 0/""/IArray().  You should:
 *  * Ensure your data never has missing fields
 *  * Ensure your usage handles these cases robustly
 *  * Use `DynHasValue` (and friends) to validate data
 *  * Use `DynGetValueOrDefault` (and friends)
 *
 * Nota Bene 2: while the method below are templated, the types are actually
 * restricted to Protobuf-compatible types.  FMI see DynamicProto.proto.
 */

// Return true if `dyn` has a single T
template <typename T>
bool DynHasValue(const ok_msg::DynamicProto &dyn);

// If `dyn` contains a single value, get it (else assert fail, or
// return 0/"" if no asserts)
template <typename T>
T DynGetValue(const ok_msg::DynamicProto &dyn);

// If `dyn` contains an array of T, get an IArray referencing it
// (else empty array)
template <typename T>
IArray<T> DynGetArray(const ok_msg::DynamicProto &dyn);

// Return true if `dyn` has a value or array for `key`
bool DynHasKey(const ok_msg::DynamicProto &dyn, const std::string &key);

// Get the value associated with `key` (else assert fail, or
// return 0/"" if no asserts)
template <typename T>
T DynGetValueForKey(
    const ok_msg::DynamicProto &dyn,
    const std::string &key);

// Get an IArray referencing the array value associated with `key`
// (else empty array)
template <typename T>
IArray<T> DynGetArrayForKey(
    const ok_msg::DynamicProto &dyn,
    const std::string &key);

// Get the Dyn for the given `key` or return null on error
const ok_msg::DynamicProto *DynGetDynForKey(
    const ok_msg::DynamicProto &dyn,
    const std::string &key);



// == Set Data ==============================================================

// Set `dyn` to contain a single value `v`
template <typename T>
void DynSetValue(ok_msg::DynamicProto &dyn, const T &v);

// Set `dyn` to contain a (copy of) the array `arr`
template <typename T>
void DynSetArray(ok_msg::DynamicProto &dyn, const IArray<T> &arr);

// Add an assignment of (`key`, `val`) to `dyn`
template <typename T>
void DynSetValueForKey(
    ok_msg::DynamicProto &dyn,
    const std::string &key,
    const T &val);

// Add an assignment of (`key`, `arr`) to `dyn`
template <typename T>
void DynSetArrayForKey(
    ok_msg::DynamicProto &dyn,
    const std::string &key,
    const IArray<T> &arr);

// Add an assignment of (`key`, `d`) to `dyn`; takes ownership of `d`
void DynSetDynForKey(
    ok_msg::DynamicProto &dyn,
    const std::string &key,
    ok_msg::DynamicProto *d);

// Add an assignment of (`key`, `d`) to `dyn`; takes ownership of `d`
void DynSetDynForKey(
    ok_msg::DynamicProto &dyn,
    const std::string &key,
    ok_msg::DynamicProto &&d);

// Add an assignment of (`key`, `d`) to `dyn`; copies `d`
void DynSetDynForKey(
    ok_msg::DynamicProto &dyn,
    const std::string &key,
    const ok_msg::DynamicProto &d);



// == Other Utils ============================================================

template <typename T>
inline
T DynGetValueOrDefault(const ok_msg::DynamicProto &dyn, T default_value) {
  return DynHasValue<T>(dyn) ? DynGetValue<T>(dyn) : default_value;
}

template <typename T>
inline
T DynGetArrayOrDefault(
    const ok_msg::DynamicProto &dyn,
    IArray<T> default_value) {
  return DynHasValue<T>(dyn) ? DynGetArray<T>(dyn) : default_value;
}

template <typename T>
inline
T DynGetValueForKeyOrDefault(
    const ok_msg::DynamicProto &dyn,
    const std::string &key,
    T default_value) {

  return
    DynHasKey(dyn, key) ? DynGetValueForKey<T>(dyn, key) : default_value;
}

// Get an IArray referencing the array value associated with `key`
// (else empty array)
template <typename T>
IArray<T> DynGetArrayForKeyOrDefault(
    const ok_msg::DynamicProto &dyn,
    const std::string &key,
    IArray<T> default_value) {

  return
      DynHasKey(dyn, key) ? DynGetArrayForKey<T>(dyn, key) : default_value;
}

std::string DynToString(const ok_msg::DynamicProto &p);

static const std::string kDynamicProtoEmptyStr = "";

namespace detail {

template <typename T>
inline
T DynZeroValue() {
  return 0;
}

template <>
inline
const std::string & DynZeroValue() {
  return kDynamicProtoEmptyStr;
}

}

} /* namespace ok */

#if OK_ENABLE_PROTOBUF
#include "ok/SerializationUtils/DynamicProto/DynamicProtoUtils-inl.hpp"
#else
#include "ok/SerializationUtils/DynamicProto/DynamicProtoUtils-nopb.hpp"
#endif

#endif /* OK_SERIALIZATIONUTILS_DYNAMICPROTO_DYNAMICPROTOUTILS_H_ */
