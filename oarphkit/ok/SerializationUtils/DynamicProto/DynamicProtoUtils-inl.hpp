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
#ifndef OK_SERIALIZATIONUTILS_DYNAMICPROTO_DYNAMICPROTOUTILS_INL_HPP_
#define OK_SERIALIZATIONUTILS_DYNAMICPROTO_DYNAMICPROTOUTILS_INL_HPP_

#ifndef OK_SERIALIZATIONUTILS_DYNAMICPROTO_DYNAMICPROTOUTILS_H_
#error "Include DynamicProtoUtils.hpp instead"
#endif

namespace ok {

// == Get Single Value ========================================================

#define DEC_GET_VALUE_PB_METHOD(RetT, PBMethodName) \
  template <> \
  inline \
  bool DynHasValue<RetT>(const ok_msg::DynamicProto &dyn) { \
    return dyn.has_##PBMethodName(); \
  } \
  \
  template <> \
  inline \
  RetT DynGetValue(const ok_msg::DynamicProto &dyn) { \
    const bool has_value = DynHasValue<RetT>(dyn); \
    OKASSERT_FATAL( \
        has_value, \
      "Missing value " + DemangledName(typeid(RetT))); \
    return has_value ? dyn.PBMethodName() : detail::DynZeroValue<RetT>(); \
  }

DEC_GET_VALUE_PB_METHOD(const std::string &, vstr);
DEC_GET_VALUE_PB_METHOD(int64_t, vint);
DEC_GET_VALUE_PB_METHOD(uint64_t, vuint);
DEC_GET_VALUE_PB_METHOD(bool, vbool);
DEC_GET_VALUE_PB_METHOD(float, vfloat);
DEC_GET_VALUE_PB_METHOD(double, vdouble);

#undef DEC_GET_VALUE_PB_METHOD

// For convenience
template <>
inline
bool DynHasValue<std::string>(const ok_msg::DynamicProto &dyn) {
  return dyn.has_vstr();
}

template <>
inline
std::string DynGetValue(const ok_msg::DynamicProto &dyn) {
  const bool has_value = DynHasValue<std::string>(dyn);
  OKASSERT_FATAL(
      has_value,
    "Missing value " + DemangledName(typeid(std::string)));
  return has_value ? dyn.vstr() : "";
}


// We have to do bytes manually since pb uses std::string as a wrapper
template <>
inline
bool DynHasValue<ROByteIArray>(const ok_msg::DynamicProto &dyn) {
  return dyn.has_vbytes();
}

template <>
inline
bool DynHasValue<ByteIArray>(const ok_msg::DynamicProto &dyn) {
  return dyn.has_vbytes();
}

template <>
inline
ROByteIArray DynGetValue(const ok_msg::DynamicProto &dyn) {
  OKASSERT_FATAL(
    DynHasValue<ROByteIArray>(dyn),
    "Missing value " + DemangledName(typeid(ROByteIArray)));
  return ROByteIArray::FascadeForContainer(dyn.vbytes());
}

template <>
inline
ByteIArray DynGetValue(const ok_msg::DynamicProto &dyn) {
  OKASSERT_FATAL(
    DynHasValue<ByteIArray>(dyn),
    "Missing value " + DemangledName(typeid(ByteIArray)));
  return ByteIArray::FascadeForBuffer(
                        (byte*) dyn.vbytes().data(),
                        dyn.vbytes().size());
}


// == Get Array ==============================================================

#define DEC_GET_ARRAY_PB_METHOD(ArrT, PBMethodName) \
  template <> \
  inline \
  IArray<ArrT> DynGetArray(const ok_msg::DynamicProto &dyn) { \
    return \
      IArray<ArrT>::FascadeForBuffer( \
        const_cast<ArrT *>(dyn.PBMethodName().data()), \
        dyn.PBMethodName().size()); \
  }

DEC_GET_ARRAY_PB_METHOD(int64_t, vsint);
DEC_GET_ARRAY_PB_METHOD(uint64_t, vsuint);
DEC_GET_ARRAY_PB_METHOD(int32_t, vsint32);
DEC_GET_ARRAY_PB_METHOD(uint32_t, vsuint32);
DEC_GET_ARRAY_PB_METHOD(float, vsfloat);
DEC_GET_ARRAY_PB_METHOD(double, vsdouble);

#undef DEC_GET_ARRAY_PB_METHOD

// We have to do string manually
template <>
inline
IArray<const std::string *> DynGetArray(const ok_msg::DynamicProto &dyn) {
  return
    IArray<const std::string *>::FascadeForBuffer(
      const_cast<const std::string **>(dyn.vsstr().data()),
      dyn.vsstr().size());
}



// == Set Single Value =======================================================

#define DEC_SET_VALUE_PB_METHOD(T, PBMethodName) \
  template <> \
  inline \
  void DynSetValue(ok_msg::DynamicProto &dyn, const T &v) { \
    dyn.set_##PBMethodName(v); \
  }

DEC_SET_VALUE_PB_METHOD(std::string, vstr);
DEC_SET_VALUE_PB_METHOD(int64_t, vint);
DEC_SET_VALUE_PB_METHOD(uint64_t, vuint);
DEC_SET_VALUE_PB_METHOD(bool, vbool);
DEC_SET_VALUE_PB_METHOD(float, vfloat);
DEC_SET_VALUE_PB_METHOD(double, vdouble);

#undef DEC_SET_VALUE_PB_METHOD



// We have to do bytes manually to copy the array
template <>
inline
void DynSetValue<ByteIArray>(ok_msg::DynamicProto &dyn, const ByteIArray &v) {
  dyn.set_vbytes(v.begin(), v.size());
}

template <>
inline
void DynSetValue<ROByteIArray>(
    ok_msg::DynamicProto &dyn,
    const ROByteIArray &v) {
  dyn.set_vbytes(v.begin(), v.size());
}


// == Set Array ==============================================================

#define DEC_SET_ARRAY_PB_METHOD(ArrT, PBMethodName) \
  template <> \
  inline \
  void DynSetArray(ok_msg::DynamicProto &dyn, const IArray<ArrT> &arr) { \
    dyn.mutable_##PBMethodName()->Resize(arr.Size(), 0); \
    memcpy( \
      dyn.mutable_##PBMethodName()->mutable_data(), \
      arr.Data(), \
      arr.Size() * sizeof(ArrT)); \
  }

DEC_SET_ARRAY_PB_METHOD(int64_t, vsint);
DEC_SET_ARRAY_PB_METHOD(uint64_t, vsuint);
DEC_SET_ARRAY_PB_METHOD(int32_t, vsint32);
DEC_SET_ARRAY_PB_METHOD(uint32_t, vsuint32);
DEC_SET_ARRAY_PB_METHOD(float, vsfloat);
DEC_SET_ARRAY_PB_METHOD(double, vsdouble);

#undef DEC_SET_ARRAY_PB_METHOD

// We have to do string manually
template <>
inline
void DynSetArray(ok_msg::DynamicProto &dyn, const IArray<std::string *> &arr) {
  dyn.mutable_vsstr()->Reserve(arr.Size());
  for (const auto &sp : arr) {
    *dyn.mutable_vsstr()->Add() = *sp;
  }
}

template <>
inline
void DynSetArray(ok_msg::DynamicProto &dyn, const IArray<std::string> &arr) {
  dyn.mutable_vsstr()->Reserve(arr.Size());
  for (const auto &s : arr) {
    *dyn.mutable_vsstr()->Add() = s;
  }
}


// == Utils ==================================================================

namespace detail {

inline
bool FindIndexOfKey(
    const ok_msg::DynamicProto &dyn,
    const std::string &key,
    size_t &o_index) {

  for (o_index = 0; o_index < dyn.k_size(); ++o_index) {
    if (dyn.k(o_index) == key) { break; }
  }
  return o_index < dyn.k_size();
}

} /* namespace detail */



// == Map Accessors ===========================================================

inline
bool DynHasKey(const ok_msg::DynamicProto &dyn, const std::string &key) {
  size_t index;
  return detail::FindIndexOfKey(dyn, key, index);
}

inline
const ok_msg::DynamicProto *DynGetDynForKey(
    const ok_msg::DynamicProto &dyn,
    const std::string &key) {

  size_t index;
  bool found = detail::FindIndexOfKey(dyn, key, index);
  OKASSERT_FATAL(
      found,
      "Missing expected value for key " + key);
  OKASSERT_FATAL(
      index < dyn.v().size(),
      "Data integrity error! No value at index that key references!");

  if (OK_LIKELY(found && index < dyn.v().size())) {
    return &dyn.v(index);
  } else {
    return nullptr;
  }
}

template <typename T>
inline
T DynGetValueForKey(const ok_msg::DynamicProto &dyn, const std::string &key) {
  auto *dv = DynGetDynForKey(dyn, key);
  OKASSERT_FATAL(dv, "No value for key " + key);
  return dv ? DynGetValue<T>(*dv) : detail::DynZeroValue<T>();
}

template <typename T>
inline
IArray<T> DynGetArrayForKey(
    const ok_msg::DynamicProto &dyn,
    const std::string &key) {

  auto *dv = DynGetDynForKey(dyn, key);
  OKASSERT_FATAL(dv, "No value for key " + key);
  return dv ? DynGetArray<T>(*dv) : IArray<T>();
}



// == Map Setters ============================================================

inline
void DynSetDynForKey(
    ok_msg::DynamicProto &dyn,
    const std::string &key,
    ok_msg::DynamicProto *d) {

  *dyn.mutable_k()->Add() = key;
  dyn.mutable_v()->AddAllocated(d);
}

inline
void DynSetDynForKey(
    ok_msg::DynamicProto &dyn,
    const std::string &key,
    ok_msg::DynamicProto &&d) {

  *dyn.mutable_k()->Add() = key;
  dyn.mutable_v()->Add()->Swap(&d);
}

inline
void DynSetDynForKey(
    ok_msg::DynamicProto &dyn,
    const std::string &key,
    const ok_msg::DynamicProto &d) {

  *dyn.mutable_k()->Add() = key;
  *dyn.mutable_v()->Add() = d;
}

template <typename T>
inline
void DynSetValueForKey(
    ok_msg::DynamicProto &dyn,
    const std::string &key,
    const T &val) {

  *dyn.mutable_k()->Add() = key;
  auto *v_dyn = dyn.mutable_v()->Add();
  DynSetValue<T>(*v_dyn, val);
}

template <typename T>
inline
void DynSetArrayForKey(
    ok_msg::DynamicProto &dyn,
    const std::string &key,
    const IArray<T> &arr) {

  *dyn.mutable_k()->Add() = key;
  auto *v_dyn = dyn.mutable_v()->Add();
  DynSetArray<T>(*v_dyn, arr);
}

} /* namespace ok */

#endif /* OK_SERIALIZATIONUTILS_DYNAMICPROTO_DYNAMICPROTOUTILS_INL_HPP_ */
