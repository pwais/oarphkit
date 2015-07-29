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

#ifndef OK_FLI_CORE_HPP_
#define OK_FLI_CORE_HPP_

#include <typeindex>
#include <typeinfo>
#include <memory>
#include <sstream>
#include <string>

#include "ok/BranchPredict.hpp"
#include "ok/Common.hpp"
#include "ok/Config.hpp"
#include "ok/Demangle.hpp"
#include "ok/fli/FLiConfig.hpp"
#include "ok/fli/Core/TPtr.hpp"

namespace ok { class SVStruct; } // Forward
namespace ok { namespace fli { struct env; } } // Forward

// == Typechecking ============================================================

// Executes a typecheck for std::type_indexs `A` and `B`
#define OK_FLI_TYPECHECK(A, B, MSG) \
  ::ok::fli::detail::Typecheck( \
                        A, B, \
                        /* For perf, wrap in a thunk. See below */ \
                        [&] { return std::string(MSG) + "\n" + \
                                        OK_OKASSERT_CODEREF();})
  /*
   * Nota bene (performance): for trivial functors (e.g. increment an int),
   * string appending dominates (and slows) performance of type checks.
   * For long chains of functors, string appending time is also a
   * factor.  Wrapping the messgae in a thunk delays comutation of
   * the message to only when a typecheck fails, providing a substantial
   * performance boost.  For example, OKFliCompositionTest is 40x
   * faster with this change.
   */

#if OK_FLI_RUNTIME_TYPECHECK_ENABLED
#define OK_FLI_RT_CHECK(A, B, MSG) \
  OK_FLI_TYPECHECK(A, B, std::string("ok::Fli RT Typecheck error: ") + MSG)
#else
#define OK_FLI_RT_CHECK(A, B, MSG) ::ok::fli::detail::ReturnTrue()
#endif

#if OK_FLI_COMPILETIME_TYPECHECK_ENABLED
#define OK_FLI_CT_CHECK(A, B, MSG) \
  OK_FLI_TYPECHECK(A, B, std::string("ok::Fli CT Typecheck error: ") + MSG)
#else
#define OK_FLI_CT_CHECK(A, B, MSG) ::ok::fli::detail::ReturnTrue()
#endif

// Convenience for returning early in a function body on typcheck error
#define OK_FLI_RET_IF_NOT(expr) do { \
  if (!(expr)) { return; } \
} while (0)

#define OK_FLI_RET_FALSE_IF_NOT(expr) do { \
  if (!(expr)) { return false; } \
} while (0)

// == Utils ==================================================================

namespace ok { namespace fli {

// Useful since typeid(void) is undefined
class Void final { };

namespace detail {

/**
 * Return true iff type a is equal to b.  May throw on typecheck failure;
 * use OK_FLI_TYPECHECK_ERROR() to define behavior.  Useful in the case
 * that exceptions are disabled but typechecking is enabled.
 */
template <typename TA, typename TB, typename MessageThunkT>
inline
bool Typecheck(const TA &a, const TB &b, const MessageThunkT &mf) {
  if (OK_LIKELY(a == b)) {
    return true;
  } else {
    // For perf, wrap message in a thunk. See above
    auto full_msg_f =
        [&] {
          return
              std::string("ok::Fli Typecheck failed ") +
              DemangledName(a) + " != " + DemangledName(b) + " " +
              mf();
    };

    #if !OK_FLI_EXCEPTIONS_ENABLED
      // No exceptions -> behave like OKAssert Check
      // (may pollute test output ..)
      OKLOG_ERROR(full_msg_f());
    #endif

    OK_FLI_TYPECHECK_ERROR_THROWS(full_msg_f());
    return false;
  }
}

inline bool ReturnTrue() { return true; }

} /* namespace detail */

// == Run-time Data ==========================================================

// Data with run-time type checks
struct rt_datumptr final {

  typedef TUPtr<void> ValuePtrType;
  ValuePtrType value;
  /* Aside: Ideally, rt_datumptr would *be* a unique_ptr (or rather a subclass
   * with associated type added), but using unique_ptr here makes
   * the code simpler and interoperates well with the majority of
   * expected use cases.
   */

  // Peligro! This struct has variable size!
  #if OK_FLI_RUNTIME_TYPECHECK_ENABLED
  std::type_index type;
  #endif

  std::string ToString() const {
    std::stringstream ss;
    ss << "rt_datumptr(";
    ss << (const void *) value.get();
    #if OK_FLI_RUNTIME_TYPECHECK_ENABLED
      ss << ";" << DemangledName(type);
    #endif
    ss << ")";
    return ss.str();
  }

  // == Construction =========================================================

  // Forbid copying; force move
  rt_datumptr(const rt_datumptr&) = delete;
  rt_datumptr& operator=(const rt_datumptr&) = delete;
  rt_datumptr(rt_datumptr &&) = default;
  rt_datumptr& operator=(rt_datumptr&&) = default;

  #if OK_FLI_RUNTIME_TYPECHECK_ENABLED
  inline rt_datumptr() noexcept
    : value(nullptr, &tptr::nonDelete),
      type(typeid(nullptr)) { }
  #else
  inline rt_datumptr() noexcept : value(nullptr, &tptr::nonDelete) { }
  #endif

  // == Expose & Wrap Data ===================================================

  template <typename T>
  inline
  T *Get() const {
    {
      const bool check = // NB: if no asserts, don't skip typechecks
          OK_FLI_RT_CHECK(
            type, typeid(T),
            "Requested data of wrong type, expected " + DemangledName(type));
      OKASSERT_FATAL(check, "Can't recover from typecheck failure");
      (void)check; // unused if no asserts
    }
    return reinterpret_cast<T *>(value.get());
  }

  template <typename T>
  inline
  T &GetRef() const {
    {
      const bool check = // NB: if no asserts, don't skip typechecks
          OK_FLI_RT_CHECK(
            type, typeid(T),
            "Requested data of wrong type, expected " + DemangledName(type));
      OKASSERT_FATAL(check, "Can't recover from typecheck failure");
      OKASSERT_FATAL(value, "Null value!");
      (void)check; // unused if no asserts
    }
    return *(reinterpret_cast<T *>(value.get()));
  }

  // Postcondition: returned datum owns `valptr`
  template <typename T>
  inline
  static rt_datumptr Wrap(std::unique_ptr<T> &&valptr) {
    rt_datumptr w;

    #if OK_FLI_RUNTIME_TYPECHECK_ENABLED
    w.type = typeid(T);
    #endif

    // Create a new uptr to void, but use a suitable deleter
    w.value =
      ValuePtrType(
          reinterpret_cast<void *>(valptr.release()),
          &tptr::doDelete<T>);

    return w;
  }

  template <typename T>
  inline
  static rt_datumptr Wrap(TUPtr<T> &&valptr) {
    rt_datumptr w;

    #if OK_FLI_RUNTIME_TYPECHECK_ENABLED
    w.type = typeid(T);
    #endif

    w.value =
      ValuePtrType(
          reinterpret_cast<void *>(valptr.release()),
          valptr.get_deleter());  // Copy the provided deleter

    return w;
  }

  // Reference, but do NOT own, `valptr`
  template <typename T>
  inline
  static rt_datumptr Reference(T *valptr) {
    rt_datumptr w;

    #if OK_FLI_RUNTIME_TYPECHECK_ENABLED
    w.type = typeid(T);
    #endif

    // Create a new uptr to void, but use a suitable deleter
    w.value =
      ValuePtrType(
        reinterpret_cast<void *>(valptr),
        &tptr::nonDelete);

    return w;
  }

  // Convert this instance into a TUPtr (and make this null)
  template <typename T>
  inline TUPtr<T> ToTPtr() {
    OKASSERT_FATAL(
      OK_FLI_RT_CHECK(
        type,
        typeid(T),
        "This datum is of type " + type.name() +
          ", can't convert to " + typeid(T).name()),
      "Can't recover from typecheck failure");

    return TUPtr<T>(
      reinterpret_cast<T *>(value.release()),
      value.get_deleter());
  }
};

// == Compile-time Data ======================================================

// Data with compile-time type checks
struct ct_datumptr final {

  typedef TUPtr<void> ValuePtrType;
  ValuePtrType value;

  // Peligro! This struct has variable size!
  #if OK_FLI_COMPILETIME_TYPECHECK_ENABLED
  std::type_index type;
  #endif

  std::string ToString() const {
    std::stringstream ss;
    ss << "ct_datumptr(";
    ss << (const void *) value.get();
    #if OK_FLI_COMPILETIME_TYPECHECK_ENABLED
      ss << ";" << DemangledName(type);
    #endif
    ss << ")";
    return ss.str();
  }

  // == Construction =========================================================

  ct_datumptr(const ct_datumptr&) = delete;
  ct_datumptr& operator=(const ct_datumptr&) = delete;
  ct_datumptr(ct_datumptr &&) = default;
  ct_datumptr& operator=(ct_datumptr&&) = default;

  #if OK_FLI_COMPILETIME_TYPECHECK_ENABLED
  inline ct_datumptr() noexcept
      : value(nullptr, &tptr::nonDelete),
        type(typeid(nullptr))
  { }
  #else
  inline ct_datumptr() noexcept : value(nullptr, &tptr::nonDelete) { }
  #endif

  // == Expose & Wrap Data ===================================================

  template <typename T>
  inline
  T *Get() const {
    {
      const bool check = // NB: if no asserts, don't skip typechecks
          OK_FLI_CT_CHECK(type, typeid(T), "Requested data of wrong type");
      OKASSERT_FATAL(check, "Can't recover from typecheck failure");
      (void)check; // unused if no asserts
    }
    return reinterpret_cast<T *>(value.get());
  }

  template <typename T>
  inline
  T &GetRef() const {
    {
      const bool check = // NB: if no asserts, don't skip typechecks
          OK_FLI_CT_CHECK(type, typeid(T), "Requested data of wrong type");
      OKASSERT_FATAL(check, "Can't recover from typecheck failure");
      OKASSERT_FATAL(OK_LIKELY(value), "Null value!");
      (void)check; // unused if no asserts
    }
    return *(reinterpret_cast<T *>(value.get()));
  }

  // Postcondition: returned datum owns `valptr`
  template <typename T>
  inline
  static ct_datumptr Wrap(std::unique_ptr<T> &&valptr) {
    ct_datumptr w;

    #if OK_FLI_COMPILETIME_TYPECHECK_ENABLED
    w.type = typeid(T);
    #endif

    // Create a new uptr to void, but use a suitable deleter
    w.value =
      ValuePtrType(
          reinterpret_cast<void *>(valptr.release()),
          &tptr::doDelete<T>);

    return w;
  }

  // Reference, but do NOT own, `valptr`
  template <typename T>
  inline
  static ct_datumptr Reference(T *valptr) {
    ct_datumptr w;

    #if OK_FLI_COMPILETIME_TYPECHECK_ENABLED
    w.type = typeid(T);
    #endif

    // Create a new uptr to void, but use a suitable deleter
    w.value =
      ValuePtrType(
        reinterpret_cast<void *>(valptr),
        &tptr::nonDelete);

    return w;
  }
};

// == Functor ================================================================

// A functor tagged with type information
class FunctorBase {
public:

  typedef TUPtr<FunctorBase> Ptr;

  FunctorBase() { }
  virtual ~FunctorBase() { }

  // Functors are opaque to non-FLi users
  virtual std::string ToString() const {
    return "FunctorBase(" + GetSignature() + ")";
  }

  virtual std::string GetSignature() const {
    return
        DemangledName(InType()) +
        "->" +
        DemangledName(OutType());
  }

  // == Compile-time Interface ===============================================

  virtual std::type_index InType() const { return typeid(nullptr); }
  virtual std::type_index OutType() const { return typeid(nullptr); }

  /**
   * Serialization to/from some message container `m` and return false
   * upon serialization error. Functors may read/write to the `env`
   * (e.g. for higher-order Functors to fetch Functor variable names,
   * or to create/mutate/destroy an external interface).  By default,
   * a Functor is stateless-- it always encodes nothing with success.
   */
  virtual bool FromEncoded(env &env, SVStruct &m) { return true; }
  virtual bool ToEncoded(env &env, SVStruct &m) { return true; }

  // Subclasses should define a RegistryAlias() to interop with Registry
  virtual std::string RegistryAlias() const { return "fli::FunctorBase"; }

  // == Run-time Interface ===================================================
  virtual rt_datumptr Call(rt_datumptr in) { return rt_datumptr(); }

};

} /* namespace fli */
} /* namespace ok */

#endif /* OK_FLI_CORE_HPP_ */
