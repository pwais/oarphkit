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
#ifndef OK_OKASSERT_HPP_
#define OK_OKASSERT_HPP_

/**
 * OKAssert: A simple, single-header, macro-based assert utility.
 *
 * To *enable* completely (disabled by default):
 *   * Compile with -DOK_OKASSERT_ENABLED=1
 *   * or #define OK_OKASSERT_ENABLED 1 before importing
 *
 * To disable backtraces in messages:
 *   * Compile with -DOK_OKASSERT_BACKTRACES=0
 *   * or #define OK_OKASSERT_BACKTRACES 0 before importing
 *
 * To disable exceptions:
 *   * Compile with -DOKASSERT_EXCEPTIONS=0
 *   * or #define OKASSERT_EXCEPTIONS 0 before importing
 *   * Note you may want to provide your own OKASSERT_FATAL_DIE()
 *     to log the failure.
 *
 * -- Fatal Asserts --
 *
 * Use OKASSERT_FATAL(cond, msg) for an assert that should crash the process
 * when `cond` is false.  When `cond` is false, OKASSERT_FATAL() will
 * send `msg` and other debug information (including a backtrace)
 * to OKASSERT_FATAL_DIE(msg).
 *
 * Assertions are provided as a macro so that they can be *elided completely*
 * from an optimized release build.  Eliding asserts in very tight loops can
 * result in 10x-100x faster code.  Furthermore, the macro grabs the
 * file and line information, which can greatly aid debugging.  Use these
 * asserts for checking null pointers, critical algorithm conditions, etc.
 *
 * To disable and elide fatal asserts (e.g. for a release build):
 *   * Compile with -DOK_OKASSERT_DISABLE_FATAL=1
 *   * #define OK_OKASSERT_DISABLE_FATAL 1 before importing
 *
 * By default, fatal asserts will throw an std::runtime_error().  To
 * customize, provide your own OKASSERT_FATAL_DIE(failmsg) function.  Before
 * importing, define your macro like this:
 * #define OKASSERT_FATAL_DIE(failmsg) do { ... } while(0)
 *
 * -- Checks --
 *
 * Use OKASSERT_CHECK(cond, msg) to check a non-fatal condition that should
 * be noted when `cond` is false. This macro *returns the value of `cond`*;
 * the user should check and handle the value accordingly.
 *
 * Checks are provided as a macro to enhance debug information and
 * enable elision of error output.  Use Checks to handle malformed user input,
 * parse/configuration errors, etc.
 *
 * To disable and elide checks:
 *   * Compile with -DOK_OKASSERT_DISABLE_CHECK=1
 *   * #define OK_OKASSERT_DISABLE_CHECK 1 before importing
 *
 * By default, check failure messages print to stderr.  To customize,
 * provide your own OKASSERT_CHECK_LOG(failmsg) function.  Before
 * importing, define your macro like this:
 * #define OKASSERT_CHECK_LOG(failmsg) do { ... } while(0)
 *
 */

#if OK_OKASSERT_ENABLED

// ==========================================================================
// Common

#ifndef OK_OKASSERT_BACKTRACES
#define OK_OKASSERT_BACKTRACES 1

#include <execinfo.h>
#include <sstream>

namespace ok { namespace detail {

inline
std::string GetBacktrace() {
  static const size_t kMaxStackFrames = 10;
  void *array[kMaxStackFrames];

  /**
   * Aside: For a more featureful (but perhaps less portable) backtrace
   * see https://github.com/bombela/backward-cpp
   *
   * Also see how Glog provides platform-dependent backtrace
   * enhancement.
   *
   * The current backtrace is usually good enough for debugging most issues
   * and is maximally portable.
   */

  size_t size = backtrace(array, kMaxStackFrames);
  char **lines = backtrace_symbols(array, size);
  std::stringstream ss;
  for (size_t l = 0; l < size && lines != NULL; ++l) {
      ss << lines[l] << std::endl;
  }
  free(lines);
  return ss.str();
}

} /* namespace detail */
} /* namespace ok */

#else

namespace ok { namespace detail {
inline std::string GetBacktrace() { return ""; }
} /* namespace detail */
} /* namespace ok */

#endif

#if __GNUC__ || __clang__
#define OK_OKASSERT_UNLIKELY(c) __builtin_expect(bool(c), false)
#else
#define OK_OKASSERT_UNLIKELY(c) (c)
#endif

#define OK_OKASSERT_S(x) #x
#define OK_OKASSERT_SS(x) OK_OKASSERT_S(x)

#define OK_OKASSERT_CODEREF() \
    "at " + \
      std::string(__FILE__ ":" OK_OKASSERT_SS(__func__) "(" OK_OKASSERT_SS(__LINE__) ")") + \
    " " + \
    (OK_OKASSERT_BACKTRACES ? (" \nBacktrace:\n" + ::ok::detail::GetBacktrace()) : "")

// ==========================================================================
// Assert

#ifndef OKASSERT_EXCEPTIONS
#define OKASSERT_EXCEPTIONS 1
#endif

#ifndef OKASSERT_EXCEPTION_TYPE
#define OKASSERT_EXCEPTION_TYPE std::runtime_error
#endif

#ifndef OKASSERT_FATAL_DIE
  #if OKASSERT_EXCEPTIONS
    #include <stdexcept>
    #define OKASSERT_FATAL_DIE(failmsg) do { \
        throw OKASSERT_EXCEPTION_TYPE((failmsg)); \
    } while(0)
  #else
    #include <iostream>
    #define OKASSERT_FATAL_DIE(failmsg) do { \
        std::cerr << std::endl; \
        std::cerr << "OKAssert Fatal Exception: " << (failmsg); \
        std::cerr << std::endl; \
        std::cerr << "*** To disable this output, declare your own ***\n"; \
        std::cerr << "*** OKASSERT_FATAL_DIE() macro before        ***\n"; \
        std::cerr << "*** importing OKAssert.hpp                   ***\n"; \
        std::cerr << std::endl; \
    } while(0)
  #endif
#endif /* OKASSERT_FATAL_DIE */

#ifndef OK_OKASSERT_DISABLE_FATAL
#define OK_OKASSERT_DISABLE_FATAL 0
#endif

#if OK_OKASSERT_DISABLE_FATAL

// Elide fatal asserts completely
#define OKASSERT_FATAL(exp, msg) do { } while(0)

#else

#include <string>
#define OKASSERT_FATAL(exp, msg) do { \
    if (OK_OKASSERT_UNLIKELY(!(exp))) { \
      /* Assertion failed: exp msg at file.cpp:func(line) Backtrace: ... */ \
      OKASSERT_FATAL_DIE( \
          "Assertion failed: " + \
          std::string(#exp) + " " + std::string(msg) + " " + \
          OK_OKASSERT_CODEREF()); \
    } \
  } while(0)

#endif

// ==========================================================================
// Check

#ifndef OKASSERT_CHECK_LOG
#include <iostream>
#define OKASSERT_CHECK_LOG(failmsg) do { \
  std::cerr << failmsg << std::endl; \
  } while(0)
#endif

namespace ok { namespace detail {

template <typename ExprT, typename MessageThunkT>
inline
const ExprT &CheckIfImpl(const ExprT &boolable, const MessageThunkT &mf) {
  if (OK_OKASSERT_UNLIKELY(!(boolable))) {
    /*
     * Nota bene: wrapping the message in a thunk delays expensive
     * string appending to this point.  For messages with many appends,
     * the perf improvement can be >= 10x.
     */
    OKASSERT_CHECK_LOG(mf());
  }
  return boolable;
}

} /* namespace detail */
} /* namespace ok */

#ifndef OK_OKASSERT_DISABLE_CHECK
#define OK_OKASSERT_DISABLE_CHECK 0
#endif

#if OK_OKASSERT_DISABLE_CHECK

// Elide check reporting
#define OKASSERT_CHECK(cond, msg) (cond)

#else

#define OKASSERT_CHECK(expect_true_cond, msg) \
  ::ok::detail::CheckIfImpl( \
      expect_true_cond, \
      /* Wrap in a thunk for perf; see CheckIfImpl() */ \
      [&] { return \
              ("Check failed: " + \
               std::string(#expect_true_cond) + " " + std::string(msg) + " " + \
               OK_OKASSERT_CODEREF()); })


#endif

#else /* OK_OKASSERT_ENABLED is false */

#define OKASSERT_FATAL(exp, msg) do { } while(0)
#define OKASSERT_CHECK(expect_true_cond, msg) do { } while(0)
#define OKASSERT_FATAL_DIE(failmsg) do { } while(0)
#define OKASSERT_CHECK_LOG(failmsg) do { } while(0)

#endif

#endif /* OK_OKASSERT_HPP_ */
