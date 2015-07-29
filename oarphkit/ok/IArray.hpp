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
#ifndef OK_IARRAY_HPP_
#define OK_IARRAY_HPP_

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "ok/BranchPredict.hpp"
#include "ok/Config.hpp"
#include "ok/Common.hpp"
#include "ok/IArray-detail.hpp"

namespace ok {

/**
 * Impute an Array Interface onto a contiguous array of `T` with known size,
 * even when some other data structure owns a **non-transferable** reference
 * to the underlying array.
 *
 * Useful for interop with non-standard containers
 * (e.g. serialization libraries).
 */
template <typename T>
class IArray {
protected:

  // All data members:
  // IArray is a pointer, size, and (possibly) an owner/deleter
  T *data_;
  size_t size_;
  detail::IOwner::IOwnerUPtr owner_;

public:

  typedef T ValueType;
  typedef std::shared_ptr<IArray<T>> SPtr;
  typedef std::unique_ptr<IArray<T>> UPtr;

  inline constexpr IArray() : data_(nullptr), size_(0), owner_(nullptr) { }

  ~IArray() { if(owner_) { owner_->Dispose(); owner_ = nullptr; } }

  // Force moves ...
  IArray(const IArray<T> &) = delete;
  IArray<T> &operator=(const IArray<T> &) = delete;
  inline IArray(IArray<T> &&other);
  inline IArray<T> &operator=(IArray<T> &&other);

  // ... or deep copies.
  IArray<T> GetDeepCopy() const;

  // ========================================================================
  // Interface to the Data

  // ==================================
  // Raw Access

  inline T* Data() noexcept { return data_; }
  inline const T* Data() const noexcept { return data_; }
  inline size_t Size() const noexcept { return size_; }
  inline bool IsEmpty() const noexcept { return data_ == nullptr; }

  // STL-like lowercase for template compatibility
  inline T* data() { return Data(); }
  inline const T* data() const { return Data(); }
  inline size_t size() const { return Size(); }
  inline bool empty() const { return IsEmpty(); }

  // ==================================
  // Array Access

  inline const T& operator[](size_t index) const {
    OKASSERT_FATAL(
      index < Size(),
      "Index: " + std::to_string(index) +
        " out of bounds (size: " + std::to_string(Size()) + ")");

    return Data()[index];
  }

  inline T& operator[](size_t index) {
    OKASSERT_FATAL(
      index < Size(),
      "Index: " + std::to_string(index) +
        " out of bounds (size: " + std::to_string(Size()) + ")");

    return Data()[index];
  }

  inline const T& at(size_t index) const {
    if (index >= Size()) {
      throw std::out_of_range(
          "Index: " + std::to_string(index) +
          " out of bounds (size: " + std::to_string(Size()) + ")");
    }

    return Data()[index];
  }

  inline T& at(size_t index) {
    if (index >= Size()) {
      throw std::out_of_range(
          "Index: " + std::to_string(index) +
          " out of bounds (size: " + std::to_string(Size()) + ")");
    }

    return Data()[index];
  }

  // ==================================
  // Iteration

  inline T* begin() { return Data(); }
  inline T* end() { return Data() + Size(); }
  inline T& front() {
    OKASSERT_FATAL(!IsEmpty(), "Empty array!"); return *Data();
  }
  inline T& back() {
    OKASSERT_FATAL(!IsEmpty(), "Empty array!"); return *(Data() + Size() - 1);
  }
  inline const T* begin() const { return Data(); }
  inline const T* end() const { return Data() + Size(); }
  inline const T& front() const {
    OKASSERT_FATAL(!IsEmpty(), "Empty array!"); return *Data();
  }
  inline const T& back() const {
    OKASSERT_FATAL(!IsEmpty(), "Empty array!");
    return *(Data() + Size() - 1);
  }

  // ==================================
  // Inspection

  // Contains array values via ok::Stringify() if T has a suitable ostream<<
  std::string ToString() const;

  // ========================================================================
  // Ownership

  // Do we own the underlying data? (or its original owner?)
  inline bool IsOwner() const noexcept {
    return (data_ != nullptr) || (owner_ != nullptr);
  }

  // Forget we own anything.  **May leak** Useful for transferring ownership.
  inline void Disown() noexcept { owner_ = nullptr; }

  // ========================================================================
  // Factories

  // ==================================
  // Owned and allocated

  inline static IArray<T> Create(size_t size) {
    IArray<T> arr;
    arr.data_ = new T[size];
    arr.owner_ = detail::TArrOwner<T>::Create(arr.data_);
    arr.size_ = size;
    return arr;
  }

  inline static IArray<T> CreateZeroed(size_t size) {
    IArray<T> arr = Create(size);
    memset((void *)arr.Data(), 0, sizeof(T) * arr.Size());
    return arr;
  }

  // ==================================
  // Reference (but don't own)

  template <typename ContainerT>
  inline static IArray<const T> FascadeForContainer(const ContainerT &c) {
    IArray<const T> arr;
    arr.data_ = (T *)c.data();
    arr.size_ = c.size();
    return arr;
  }

  template <typename ContainerT>
  inline static IArray<T> FascadeForContainer(ContainerT &c) {
    IArray<T> arr;
    arr.data_ = (T *)c.data();
    arr.size_ = c.size();
    return arr;
  }

  inline static IArray<T> FascadeForBuffer(T *buffer, size_t size) {
    OKASSERT_FATAL((buffer == nullptr) == (size == 0), "Inconsistent buffer!");
    IArray<T> arr;
    arr.data_ = buffer;
    arr.size_ = size;
    return arr;
  }

  inline static IArray<T> FascadeForRange(T *begin, T *end) {
    OKASSERT_FATAL(begin <= end, "Inconsistent range!");
    IArray<T> arr;
    arr.data_ = (begin == end) ? nullptr : begin;
    arr.size_ = end - begin;
    return arr;
  }

  // ==================================
  // Take Ownership

  template <typename ContainerT>
  inline static IArray<T> WrappingContainer(ContainerT &&c) {
    return WrappingContainer(new ContainerT(std::move(c)));
  }

  template <typename ContainerT>
  inline static IArray<T> WrappingContainer(ContainerT *cptr) {
    OKASSERT_FATAL(cptr, "Null pointer!");
    IArray<T> arr;
    arr.owner_ = detail::ContainerTOwner<ContainerT>::Create(cptr);
    arr.data_ = (T *)cptr->data();
    arr.size_ = cptr->size();
    return arr;
  }

  template <typename ContainerT>
  inline static IArray<T> WrappingContainer(std::shared_ptr<ContainerT> cptr) {
    OKASSERT_FATAL(cptr, "Null pointer!");
    IArray<T> arr;
    arr.owner_ = detail::SharedTOwner<ContainerT>::Create(cptr);
    arr.data_ = (T *)cptr->data();
    arr.size_ = cptr->size();
    return arr;
  }

  // Releases cptr and takes owernship of the underlying container!
  template <typename ContainerT>
  inline
  static IArray<T> WrappingContainer(std::unique_ptr<ContainerT> &&cptr) {
    OKASSERT_FATAL(cptr, "Null pointer!");
    return WrappingContainer(cptr.release());
  }

  inline static IArray<T> WrappingBuffer(
      T *buffer,
      size_t size,
      std::function<void()> deleter) {

    IArray<T> arr;
    arr.owner_ = detail::CallableDeleter::Create(deleter);
    arr.data_ = buffer;
    arr.size_ = size;
    return arr;
  }

  inline static IArray<T>::UPtr ToUPtr(IArray<T> &&arr) {
    return IArray<T>::UPtr(new IArray<T>(std::move(arr)));
  }

  inline static IArray<T>::SPtr ToSPtr(IArray<T> &&arr) {
    return IArray<T>::SPtr(new IArray<T>(std::move(arr)));
  }
};

// For convenience
typedef IArray<byte> ByteIArray;
typedef ByteIArray::UPtr ByteIArrayUPtr;
typedef IArray<const byte> ROByteIArray;

} /* namespace ok */

#include "ok/IArray-inl.hpp"

#endif /* OK_IARRAY_HPP_ */
