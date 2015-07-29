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
#ifndef OK_CONFIG_HPP_
#define OK_CONFIG_HPP_

// TODO: reconsider config.h compile-time options
//#include "ok/config.h"

// Defaults for compile-time constants (mostly to make IDEs happy)
#ifndef OK_MAJOR_VERSION
#define OK_MAJOR_VERSION 0
#endif
#ifndef OK_MINOR_VERSION
#define OK_MINOR_VERSION 0
#endif
#ifndef OK_PATCH_VERSION
#define OK_PATCH_VERSION 0
#endif
#ifndef OK_VERSION
#define OK_VERSION "unknown"
#endif

// Can Oarphkit log to stderrr?
#ifndef OK_LOGERR_ENABLED
#define OK_LOGERR_ENABLED 1
#endif

// Can Oarphkit use exceptions?
#ifndef OK_DISABLE_EXCEPTIONS
#define OK_DISABLE_EXCEPTIONS 0
#endif
// TODO: cut exceptions for unordered_map too? e.g. .at() calls

#define OK_FLI_EXCEPTIONS_ENABLED !OK_DISABLE_EXCEPTIONS
#define OKASSERT_EXCEPTIONS !OK_DISABLE_EXCEPTIONS

#ifndef OK_OKLOG_ENABLED
#define OK_OKLOG_ENABLED OK_LOGERR_ENABLED
#endif
#include "ok/OKLog.hpp"

// Can Oarphkit have assertions and checks?
#ifndef OK_ASSERTIONS_AND_CHECKS
#define OK_OKASSERT_ENABLED 1

// Log checks using OKLOG
#if OK_LOGERR_ENABLED
  #ifndef OKASSERT_CHECK_LOG
  #define OKASSERT_CHECK_LOG(failmsg) do { OKLOG_NOFILE(failmsg); } while(0)
  #endif
#endif

#else /* ! OK_ASSERTIONS_AND_CHECKS */

#define OK_OKASSERT_DISABLE_FATAL 1
#define OK_OKASSERT_DISABLE_CHECK 1

// But enable the rest of OKAssert (e.g. backtraces)
#define OK_OKASSERT_ENABLED 1

#endif

#include "ok/OKAssert.hpp"

// Can we use protobuf?
#ifndef OK_ENABLE_PROTOBUF
#define OK_ENABLE_PROTOBUF 1
#endif

// Can we use capnp?
#ifndef OK_ENABLE_CAPNP
#define OK_ENABLE_CAPNP 1
#endif

#endif /* OK_CONFIG_HPP_ */
