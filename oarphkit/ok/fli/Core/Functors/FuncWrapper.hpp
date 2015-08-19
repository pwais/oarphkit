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

#ifndef OK_FLI_CORE_FUNCTORS_FUNCWRAPPER_HPP_
#define OK_FLI_CORE_FUNCTORS_FUNCWRAPPER_HPP_

#include <functional>
#include <memory>
#include <type_traits>

#include "ok/fli/Core/Core.hpp"
#include "ok/TemplateFu.hpp"

namespace ok { namespace fli {

/*
 * This module contains a Functor that wraps ordinary C++ functions
 * (i.e. typically pure, unbound functions).  See the OK_FLI_DECLARE_FW(func)
 * macro below.
 *
 * If your function has state (and it's bound to a class ...), subclass
 * from a different Functor base class.
 *
 * If your function has arity greater than one, create a struct encapsulating
 * those parameters and use that struct as a single parameter to the wrapped
 * function.  (I.e., use a trick from Thrift, where method parameters are
 * implemented as auto-generated Thrift structs [and all functions are
 * actually unary]).
 *
 * We try to support a variety of function parameter and return types.  See
 * unit tests for details & examples.
 */

namespace fw {

template <typename T>
struct func_traits : func_traits<decltype(&T::operator())> {};

template <class Ret, class Arg>
struct func_traits<Ret(Arg)> {
  using result_type = Ret;
  using argument_type = Arg;
};

template <class Ret, class Arg>
struct func_traits<std::function<Ret(Arg)>> {
  using result_type = Ret;
  using argument_type = Arg;
};

template <class Ret>
struct func_traits<Ret(void)> {
  using result_type = Ret;
  using argument_type = void;
};

template <class Arg>
struct func_traits<void(Arg)> {
  using result_type = void;
  using argument_type = Arg;
};

template<typename T>
struct is_uptr {
  template <
    typename U=T,
    typename std::enable_if<
      std::is_same<U, std::unique_ptr<typename U::element_type>>::value
          // NB: we'll convert to TPtr, so we need the matched uptr type
          // to use default_delete, NOT some other delete
          // (i.e. std::unique_ptr<U, D> )
        >::type* = nullptr>
  static auto check(int) -> std::true_type;

  template <typename U=T>
  static auto check(bool) -> std::false_type;

  static constexpr bool value = decltype(check<T>(0))::value;
};

template <typename T>
struct remove_ptr_or_uptr {
  template <
    typename U=T,
    typename std::enable_if<is_uptr<U>::value>::type* = nullptr>
  static auto check(int) -> std::remove_reference<decltype(*std::declval<U>())>;

  template <typename U=T>
  static auto check(bool) -> std::remove_pointer<U>;

  using type = typename decltype(check<T>(0))::type;
};

template <typename T>
struct to_fli_void {
  template <typename U>
  struct type_of { typedef U type; }; // TODO: better way?

  template <
    typename U=T,
    typename std::enable_if<std::is_void<U>::value>::type* = nullptr>
  static auto convert(int) -> type_of<Void>;

  template <typename U=T>
  static auto convert(bool) -> type_of<U>;

  using type = typename decltype(convert<T>(0))::type;
};

} /* namespace fw */

// Actual Functor definition
template <typename FuncT>
class FuncWrapper : public FunctorBase {

public:
  FuncWrapper() { }
  virtual ~FuncWrapper() { }

  typedef FuncT FuncType;
  typedef typename fw::func_traits<FuncType>::argument_type FuncArgType;
  typedef typename fw::func_traits<FuncType>::result_type FuncResType;

  using InValType =
    typename std::remove_pointer<
      typename std::remove_reference<
        typename fw::to_fli_void<FuncArgType>::type>::type>::type;

  using OutValType =
    typename fw::remove_ptr_or_uptr<
      typename fw::to_fli_void<FuncResType>::type>::type;

  typedef TUPtr<OutValType> OutTypePtr;

  virtual std::string ToString() const {
    return "FuncWrapper(" + FunctorBase::GetSignature() + ")";
  }

  virtual std::string RegistryAlias() const { return "fli::FuncWrapper"; }

  virtual std::type_index InType() const override {
    return typeid(InValType);
  }

  virtual std::type_index OutType() const override {
    return typeid(OutValType);
  }

  // Utility, mainly for testing
  inline OutTypePtr Call(InValType *arg=nullptr) {
    return Call(arg, true);
  }

protected:

  std::function<FuncT> func_; // The only data / state: a ref to our FuncT

protected:

  // Core FLi Call() interface
  virtual rt_datumptr Call(rt_datumptr in) override {
    return rt_datumptr::Wrap<OutValType>(Call(in.Get<InValType>(), true));
      // Forwards Transform::Call() to a FuncT-wrapping member, whose
      // actual type is dependent on FuncT
  }

  /*
   * The members below instantiate mutually-exclusively due to
   * template parameters.  Each declaration wraps a different
   * type of FuncT and handles param & return values (if any)
   * in order to interop with the rest of FLi.
   *
   * NB: we include the dummy `FuncArgT` and `FuncResT` template
   * variables to make SFINAE perform properly; without them,
   * all the declarations are effectively explicitly
   * instantiated.
   *
   * Notes:
   *  * If the return type is std::unique_ptr, we convert directly to a TUPtr.
   *  * If the function returns a pointer (i.e. an allocated object), FLi
   *  takes ownership of that pointer.  If this behavior is not desirable:
   *     * Make your function return a reference
   *     * Make your function return a std::{shared,weak}_ptr
   *     * Make your function return a Maybe (C++17? or see Captain Proto / KJ)
   *     * Make your function return a fli::TUPtr
   *     * In whatever fli::Functor that calls this FuncWrapper instance,
   *        immediately .get() and .release() the underlying pointer
   *        from the FuncWrapper::Call()-returned TUPtr
   *  * If the return type is move constructible (and *not* a pointer), we
   *      will utilize that move constructor so that FLi can obtain a
   *      pointer to the returned value.
   *  * Non-pointer parameter types should be movable, else their copy ctor
   *      might be called.
   */

  // void -> void
  template <
    typename FuncArgT=typename fw::func_traits<FuncType>::argument_type,
    typename FuncResT=typename fw::func_traits<FuncType>::result_type,
    typename std::enable_if<
              std::is_void<FuncArgT>::value &&
              std::is_void<FuncResT>::value
            >::type* = nullptr>
  OutTypePtr Call(InValType *arg, bool) {
    func_();
    return NullTUPtr<OutValType>();
  }

  // void -> is_move_constructible
  template <
    typename FuncArgT=typename fw::func_traits<FuncType>::argument_type,
    typename FuncResT=typename fw::func_traits<FuncType>::result_type,
    typename std::enable_if<
              std::is_void<FuncArgT>::value &&
              !std::is_void<FuncResT>::value &&
              std::is_move_constructible<FuncResT>::value &&
              !std::is_pointer<FuncResT>::value &&
              !fw::is_uptr<FuncResT>::value
            >::type* = nullptr>
  OutTypePtr Call(InValType *arg, bool) {
    return MakeOwned<OutValType>(new FuncResT(func_()));
  }

  // void -> (pointer || uptr)
  template <
    typename FuncArgT=typename fw::func_traits<FuncType>::argument_type,
    typename FuncResT=typename fw::func_traits<FuncType>::result_type,
    typename std::enable_if<
              std::is_void<FuncArgT>::value &&
              !std::is_void<FuncResT>::value &&
              (std::is_pointer<FuncResT>::value
                  || fw::is_uptr<FuncResT>::value)
            >::type* = nullptr>
  OutTypePtr Call(InValType *arg, bool) {
    return MakeOwned<OutValType>(func_());
      // NB: FLi takes ownership of the pointer
  }

  // non-void, non-pointer -> void
  template <
    typename FuncArgT=typename fw::func_traits<FuncType>::argument_type,
    typename FuncResT=typename fw::func_traits<FuncType>::result_type,
    typename std::enable_if<
              !std::is_pointer<FuncArgT>::value &&
              !std::is_void<FuncArgT>::value &&
              std::is_void<FuncResT>::value
          >::type* = nullptr>
  OutTypePtr Call(InValType *arg, bool) {
    OKASSERT_FATAL(arg, "Expected non-null arg");
    if (OK_LIKELY(arg)) {
      // TODO: docs about move
      func_(std::move(*arg));
    }
    return NullTUPtr<OutValType>();
  }

  // move_constructible -> move_constructible
  template <
    typename FuncArgT=typename fw::func_traits<FuncType>::argument_type,
    typename FuncResT=typename fw::func_traits<FuncType>::result_type,
    typename std::enable_if<
              !std::is_pointer<FuncArgT>::value &&
              !std::is_void<FuncArgT>::value &&
              std::is_move_constructible<FuncResT>::value &&
              !(std::is_pointer<FuncResT>::value ||
                  fw::is_uptr<FuncResT>::value)
            >::type* = nullptr>
  OutTypePtr Call(InValType *arg, bool) {
    OKASSERT_FATAL(arg, "Expected non-null arg");
    if (OK_LIKELY(arg)) {
      // TODO: docs about move
      return MakeOwned<OutValType>(
              new FuncResT(
                func_(
                  std::move(*arg))));
    } else {
      return NullTUPtr<OutValType>();
    }
  }

  // non-void, non-pointer -> (pointer || uptr)
  template <
    typename FuncArgT=typename fw::func_traits<FuncType>::argument_type,
    typename FuncResT=typename fw::func_traits<FuncType>::result_type,
    typename std::enable_if<
              !std::is_pointer<FuncArgT>::value &&
              !std::is_void<FuncArgT>::value &&
              (std::is_pointer<FuncResT>::value ||
                  fw::is_uptr<FuncResT>::value)
            >::type* = nullptr>
  OutTypePtr Call(InValType *arg, bool) {
    OKASSERT_FATAL(arg, "Expected non-null arg");
    if (OK_LIKELY(arg)) {
      FuncArgT &argRef = *arg;
      return MakeOwned<OutValType>(func_(argRef));
        // NB: FLi takes ownership of the pointer
    } else {
      return NullTUPtr<OutValType>();
    }
  }

  // pointer -> void
  template <
    typename FuncArgT=typename fw::func_traits<FuncType>::argument_type,
    typename FuncResT=typename fw::func_traits<FuncType>::result_type,
    typename std::enable_if<
              std::is_pointer<FuncArgT>::value &&
              std::is_void<FuncResT>::value
          >::type* = nullptr>
  OutTypePtr Call(InValType *arg, bool) {
    OKASSERT_FATAL(arg, "Expected non-null arg");
    if (OK_LIKELY(arg)) {
      func_(arg);
    }
    return NullTUPtr<OutValType>();
  }

  // pointer -> (pointer || uptr)
  template <
    typename FuncArgT=typename fw::func_traits<FuncType>::argument_type,
    typename FuncResT=typename fw::func_traits<FuncType>::result_type,
    typename std::enable_if<
              std::is_pointer<FuncArgT>::value &&
              (std::is_pointer<FuncResT>::value ||
                  fw::is_uptr<FuncResT>::value)
            >::type* = nullptr>
  OutTypePtr Call(InValType *arg, bool) {
    return MakeOwned<OutValType>(func_(arg));
      // NB: FLi takes ownership of the pointer
  }

};

} /* namespace fli */
} /* namespace ok */

/**
 * Declares a wrapper class with name `WrapperName` for function `func`
 * with Registry name `Regname`.
 */
#define OK_FLI_DECLARE_FW(WrapperName, func, Regname) \
  class WrapperName : public ::ok::fli::FuncWrapper<decltype(func)> { \
  public: \
    WrapperName() { func_ = &func; } \
    virtual ~WrapperName() { } \
    virtual std::string ToString() const { \
      return \
          std::string("FuncWrapper.") + OK_XSTRINGIFY(WrapperName) + \
            "(" + FunctorBase::GetSignature() + ")"; \
    } \
    virtual std::string RegistryAlias() const { \
      return Regname; \
    } \
  };

#endif /* OK_FLI_CORE_FUNCTORS_FUNCWRAPPER_HPP_ */
