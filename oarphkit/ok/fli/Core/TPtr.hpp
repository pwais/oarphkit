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
#ifndef OK_TPTR_HPP_
#define OK_TPTR_HPP_

#include <functional>
#include <memory>
#include <type_traits>

namespace ok { namespace fli { namespace tptr {

template <typename T>
void doDelete(void *ptr) noexcept {
  delete reinterpret_cast<T *>(ptr);
}

template <typename T>
void doDeleteArray(void *ptr) noexcept {
  delete[] reinterpret_cast<T *>(ptr);
}

// A hack to make unique_ptr a weak, unowned pointer
inline void nonDelete(void *ptr) noexcept { }

} /* namespace tptr */

/**
 * A unique_ptr that doesn't (necessarily) own the pointee (allows
 * weak references) and is capable of owning void (with pointee type
 * encoded into the deleter).  See detailed discussion below.
 */
template <typename T>
using TUPtr = std::unique_ptr<T, decltype(&tptr::doDelete<T>)>;

template <typename T>
inline
TUPtr<T> MakeOwned(std::unique_ptr<T> &&ptr) {
  return TUPtr<T>(ptr.release(), &tptr::doDelete<T>);
}

template <typename T>
inline
TUPtr<T> MakeOwned(T *ptr) {
  return TUPtr<T>(ptr, &tptr::doDelete<T>);
}

template <typename T>
inline
TUPtr<T> MakeOwned(T *ptr, decltype(&tptr::doDelete<T>) deleter) {
  return TUPtr<T>(ptr, deleter);
}

template <typename ArrT>
inline
TUPtr<ArrT> MakeOwnedArray(ArrT *ptr) {
  return TUPtr<ArrT>(ptr, &tptr::doDeleteArray<ArrT>);
}

template <typename T>
inline
TUPtr<T> MakeOwnedCopy(const T &v) {
  return TUPtr<T>(new T(v), &tptr::doDelete<T>);
}

template <typename T>
inline
TUPtr<T> MakeUnOwned(T *ptr) noexcept {
  return TUPtr<T>(ptr, &tptr::nonDelete);
}

template <typename T>
inline
TUPtr<T> MakeUnOwned(const TUPtr<T> &p) noexcept {
  return MakeUnOwned<T>(p.get());
}

template <
  typename T,
  typename B,
  typename std::enable_if<std::is_base_of<B, T>::value>::type* = nullptr>
inline
TUPtr<T> MakeUnOwned(const TUPtr<B> &p) {
  return MakeUnOwned<T>(static_cast<T *>(p.get()));
}

template <typename T>
inline
TUPtr<T> NullTUPtr() noexcept {
  return MakeUnOwned<T>(nullptr);
}

template <typename T>
inline
std::shared_ptr<T> ToShared(TUPtr<T> &&p) {
  return std::shared_ptr<T>(p.release(), p.get_deleter());
}

} /* namespace fli */
} /* namespace ok */

/**
 * Discussion
 *
 * Why TPtr?
 *  * In FLi, we need to own pointers of type `void` yet ensure we clean up
 *    data properly.  TPtr makes std::unique_ptr<void, decltype(deleter<T>)>
 *    feasible; i.e. type data is encoded entirely in the deleter.  Moreover,
 *    the type system will compare the deleter signature by decltype,
 *    so unique_ptrs to different Ts effectively have the same type.
 *
 *  * Furthermore, FLi needs to support weak pointers to accept user-owned
 *    data as input.  In order to make the code simpler, rather than
 *    accepting raw T*, we allow TPtr to use a null deleter.
 *
 *  * STL might recommend template specializatins of std::default_delete,
 *    but we do *NOT* want to override std::default_delete<T> for T because
 *    TPtr deleters have special behavior (e.g. they take `void *` as input or
 *    don't delete *at all*).
 *
 *  * TPtr uses function pointer deleters for efficiency; deleter callables
 *    don't need to be allocated for individual pointers.
 *
 *  * We could write our own unique_ptr, ala kj::Own<T>, but we maintain
 *    a useful compatibility with the STL in this solution.
 *
 * What TPtr *cannot* do:
 *  * TPtr is not useful for exposing a T* as a T'*.  For example,
 *    IArray provides a facility for wrapping a container (e.g. std::vector)
 *    that owns (and cannot release) a buffer and providing a
 *    convenient movable interface for the buffer.  Thus IArray might
 *    expose a T* but actually own a ContainerT *.  TPtr could be
 *    applicable to this situation if TPtr used deleter /instances/
 *    (e.g. TPtr could contain a deleter that owns and deletes
 *    ContainterT * properly), but we'd rather not sacrifice TPtr's
 *    efficient use of function pointer deleters.
 *
 *  * TPtr assumes the user will use `new` as an allocator.  If you're using
 *    arenas and/or a custom allocator, you'd need to define your own
 *    deleter function or (more likely) not use TPtr
 *    (as you might need a deleter specific to the pointer's arena
 *    instance).
 */

#endif /* OK_TPTR_HPP_ */
