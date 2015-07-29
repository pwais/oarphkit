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
#ifndef OK_IARRAY_DETAIL_HPP_
#define OK_IARRAY_DETAIL_HPP_

#include <functional>

namespace ok { namespace detail {

// A object capable of disposing of another
class IOwner {
public:
  typedef std::unique_ptr<IOwner> IOwnerUPtr;

  // Disposing the owner should *not* necessarily Dispose() the value
  virtual ~IOwner() { }

  // Dispose of the owned value (and thus disown())
  virtual void Dispose() { }

};

template <typename T>
class SharedTOwner : public IOwner {
public:

  static IOwnerUPtr Create(const std::shared_ptr<T> &p) noexcept {
    IOwnerUPtr io(new SharedTOwner());
    SharedTOwner *owner = reinterpret_cast<SharedTOwner *>(io.get());
    owner->p_ = p;
    return io;
  }

  SharedTOwner() noexcept { }
  virtual ~SharedTOwner() { }

  explicit SharedTOwner(std::shared_ptr<T> &&p) : p_(std::move(p)) { }
  explicit SharedTOwner(const std::shared_ptr<T> &p) : p_(p) { }

  virtual void Dispose() override { p_.reset(); }


protected:
  std::shared_ptr<T> p_;
};

template <typename ContainerT>
class ContainerTOwner : public IOwner {
public:
  ContainerTOwner() noexcept : c_(nullptr) { }
  virtual ~ContainerTOwner() { }

  static IOwnerUPtr Create(ContainerT *c) noexcept {
    IOwnerUPtr io(new ContainerTOwner<ContainerT>());
    ContainerTOwner<ContainerT> *owner =
        reinterpret_cast<ContainerTOwner<ContainerT> *>(io.get());
    owner->c_ = c;
    return io;
  }

  virtual void Dispose() override { delete c_; c_ = nullptr; }

protected:
  ContainerT *c_;

private:
  ContainerTOwner(const ContainerTOwner<ContainerT>&) = delete;
  ContainerTOwner<ContainerT>& operator=(
    const ContainerTOwner<ContainerT>&) = delete;
  ContainerTOwner(ContainerTOwner<ContainerT> &&) = delete;
  ContainerTOwner& operator=(ContainerTOwner &&) = delete;
};

template <typename T>
class TArrOwner : public IOwner {
public:
  TArrOwner() noexcept : a_(nullptr) { }
  virtual ~TArrOwner() { }

  static IOwnerUPtr Create(T *a) noexcept {
    IOwnerUPtr io(new TArrOwner());
    TArrOwner *owner = reinterpret_cast<TArrOwner *>(io.get());
    owner->a_ = a;
    return io;
  }

  virtual void Dispose() override { delete[] a_; a_ = nullptr; }

protected:
  T *a_;

private:
  TArrOwner(const TArrOwner<T>&) = delete;
  TArrOwner<T>& operator=(
    const TArrOwner<T>&) = delete;
  TArrOwner(TArrOwner<T> &&) = delete;
  TArrOwner& operator=(TArrOwner &&) = delete;
};

class CallableDeleter : public IOwner {
public:
  CallableDeleter() { }
  virtual ~CallableDeleter() { }

  static IOwnerUPtr Create(const std::function<void()> &f) {
    IOwnerUPtr io(new CallableDeleter());
    CallableDeleter *owner = reinterpret_cast<CallableDeleter *>(io.get());
    owner->f_ = f;
    return io;
  }

  virtual void Dispose() override { f_(); }

protected:
  std::function<void()> f_;
};

} /* namespace detail */
} /* namespace ok */

#endif /* OK_IARRAY_DETAIL_HPP_ */
