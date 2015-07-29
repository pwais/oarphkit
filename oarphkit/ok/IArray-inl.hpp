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
#ifndef OK_IARRAY_INL_HPP_
#define OK_IARRAY_INL_HPP_

#ifndef OK_IARRAY_HPP_
#error "Include IArray.hpp instead"
#endif

#include <sstream>

#include "ok/Demangle.hpp"
#include "ok/Stringify.hpp"

namespace ok {

template <typename T>
inline
IArray<T>::IArray(IArray<T> &&other)
  : data_(other.data_),
    size_(other.size_),
    owner_(std::move(other.owner_)) {

  other.data_ = nullptr;
  other.size_ = 0;
}

template <typename T>
inline
IArray<T>& IArray<T>::operator=(IArray<T> &&other) {
  if(owner_ && OK_LIKELY(owner_ != other.owner_)) {
    owner_->Dispose();
    owner_ = nullptr;
  }

  data_ = other.data_;
  size_ = other.size_;
  owner_ = std::move(other.owner_);

  other.data_ = nullptr;
  other.size_ = 0;

  return *this;
}

template <typename T>
inline
IArray<T> IArray<T>::GetDeepCopy() const {
  if (OK_UNLIKELY(IsEmpty())) { return IArray<T>(); }
  IArray<T> cp = IArray<T>::Create(Size());
  memcpy(cp.data_, data_, sizeof(T) * size_);
  return cp;
}



template <typename T>
inline
std::string IArray<T>::ToString() const {
  std::stringstream ss;

  /*
   * Build e.g. "IArray(sz:0;data=0xff;owner:1;vals: a, b, c )"
   */

  ss << "IArray<" << DemangledName(typeid(T)) << ">(sz:" << Size();

  // Aside: for nullptr, OSX prints "0x0" but Ubuntu
  // prints "(nil)", so we normalize
  if (IsEmpty()) {
    ss << ";data:null";
  } else {
    ss << ";data:" << ((const void *) Data());
  }

  ss << ";owner:" << IsOwner() << ";vals: ";

  // Print values
  if (OK_LIKELY(!IsEmpty())) {
    auto front_str = Stringify<ValueType>(Data()[0]);
    if (front_str == strify::kOpaque) {
      ss << "[opaque vals]";
    } else {
      ss << front_str;
      for (size_t i = 1; i < Size(); ++i) {
        ss << ", " << Stringify<ValueType>(Data()[i]);
      }
    }
  }

  ss << " )";
  return ss.str();
}

} /* namespace ok */

#endif /* OK_IARRAY_INL_HPP_ */
