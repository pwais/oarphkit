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
#ifndef OK_FLI_FLICONFIG_HPP_
#define OK_FLI_FLICONFIG_HPP_

/**
 * Use to elide *all* type checking.
 *   ** DANGEROUS **
 * Only do this for full-race performance
 * on Sessions known to "compile" corectly.
 */
#ifndef OK_FLI_TYPECHECK_ENABLED
#define OK_FLI_TYPECHECK_ENABLED 1
#endif

/**
 * Perform typechecks at FLi "run" time? Where "run" time is:
 *  * During a FLi Session Run(), Push(), Pop(), etc
 *  * During invocation of library Functors
 *
 * Safe to disable, especially for Sessions known to compile.
 */
#ifndef OK_FLI_RUNTIME_TYPECHECK_ENABLED
#define OK_FLI_RUNTIME_TYPECHECK_ENABLED OK_FLI_TYPECHECK_ENABLED
#endif

/**
 * Perform typechecks at FLi "compile" time? Where "compile" time is:
 *  * During a FLi Session construction or Compile() call
 *  * During the construction of a Functor
 *
 * Dangerous to disable; do so only if "compile" time is significant.
 */
#ifndef OK_FLI_COMPILETIME_TYPECHECK_ENABLED
#define OK_FLI_COMPILETIME_TYPECHECK_ENABLED OK_FLI_TYPECHECK_ENABLED
#endif

/**
 * Throw exceptions upon typcheck failures
 */
#ifndef OK_FLI_EXCEPTIONS_ENABLED
#define OK_FLI_EXCEPTIONS_ENABLED 1
#endif

#if OK_FLI_EXCEPTIONS_ENABLED
  #ifndef OK_FLI_TYPECHECK_ERROR_EXCEPTION_TYPE
  #include <stdexcept>
  #define OK_FLI_TYPECHECK_ERROR_EXCEPTION_TYPE std::runtime_error
  #endif

  /**
   * Override with custom macro to define if and how typecheck errors
   * throw exceptions
   */
  #ifndef OK_FLI_TYPECHECK_ERROR_THROWS
  #define OK_FLI_TYPECHECK_ERROR_THROWS(failmsg) do { \
      throw OK_FLI_TYPECHECK_ERROR_EXCEPTION_TYPE((failmsg)); \
    } while(0)
  #endif
#else

#ifndef OK_FLI_TYPECHECK_ERROR_THROWS
#define OK_FLI_TYPECHECK_ERROR_THROWS(failmsg) do { } while(0)
#endif

#endif

#endif /* OK_FLI_FLICONFIG_HPP_ */
