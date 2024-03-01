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
#ifndef OK_OKLOG_HPP_
#define OK_OKLOG_HPP_

/**
 * OKLog: A simple, single-header, macro-based logging library.
 *
 * Use the OKLOG() macro to log an expression:
 *   OKLOG("oarph oarph! I have " << 5 << " fish.")
 * will print to (by default) stderr:
 *   "OKLOG Filename.cpp:foo(42): oarph oarph! I have 5 fish.
 *
 * Use other log levels that can be disabled (i.e. *elided*) separately:
 *   OKLOG_ERROR("oarph! your fish is dead!")
 *   OKLOG_DEBUG("pssst... here's a fish!")
 *
 * Use the OKLOG_ELAPSED() macro to time a code block:
 *   ok::TimePoint start;
 *   DoExpensiveOarphing();
 *   OKLOG_ELAPSED(start, "oarphing runtime")
 * will log:
 *   "OKLOG Filename.cpp:foo(42): oarphing runtime [Elapsed 1ms (1000hz)]"
 * You may override the OK_OKLOG_TIME_MESSAGE(elapsed_usec) macro
 * to customize how the "[Elapsed ...]" suffix appears.
 *
 * Macro-based Configuration:
 * To *enable* (logging is *disabled* by default):
 *   * Compile with -DOK_OKLOG_ENABLED=1
 *   * #define OK_OKLOG_ENABLED 1 before importing
 *
 * To disable file information:
 *   * Compile with -DOK_OKLOG_FILEINFO=0
 *   * #define OK_OKLOG_FILEINFO 0 before importing
 *
 * To customize log tag:
 *   * Compile with -DOK_OKLOG_LOGTAG=YourTag
 *   * #define OK_OKLOG_LOGTAG "YourTag" before importing
 *
 * To log to an std::ostream other than stderr:
 *   * #define OK_OKLOG_OSTREAM your_ostream
 *
 * To disable (and *elide*) different log levels, use:
 *   * OK_OKLOG_ENABLE_DEBUG=0 -> disable OKLOG_DEBUG()
 *   * OK_OKLOG_ENABLE_ERROR=0 -> disable OKLOG_ERROR()
 *
 * To disable timing:
 *   * Compile with -DOK_OKLOG_ENABLE_TIME=0
 *   * #define OK_OKLOG_ENABLE_TIME 0 before importing
 *
 * To customize OKLOG_ELAPSED() suffix:
 *   * #define OK_OKLOG_TIME_MESSAGE(elapsed_usec) \
 *       "your expression as a function of elapse microseconds (uint64_t)"
 */

#if OK_OKLOG_ENABLED

// ==========================================================================
// Macro Configuration

#ifndef OK_OKLOG_OSTREAM
#include <iostream>
#define OK_OKLOG_OSTREAM std::cerr
#endif

#ifndef OK_OKLOG_FILEINFO
#define OK_OKLOG_FILEINFO 1
#endif

#ifndef OK_OKLOG_LOGTAG
#define OK_OKLOG_LOGTAG "OKLOG"
#endif

#ifndef OK_OKLOG_ENABLE_DEBUG
#define OK_OKLOG_ENABLE_DEBUG 1
#endif

#ifndef OK_OKLOG_ENABLE_ERROR
#define OK_OKLOG_ENABLE_ERROR 1
#endif

#ifndef OK_OKLOG_ENABLE_TIME
#define OK_OKLOG_ENABLE_TIME 1
#endif

// ==========================================================================
// Utilities

#if OK_OKLOG_FILEINFO

// Return just the filename from __FILE__, since full paths can be unhelpful
#include <string.h>
#define OK_GETFILENAME() std::string(strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

// Return, e.g. "Filename.cpp:foo(42):"
#define OKLOG_FILEINFO() \
  OK_GETFILENAME() << ":" << __func__ << "(" << __LINE__ << "):"
#else
#define OKLOG_FILEINFO() ""
#endif

// ==========================================================================
// Core Logging Macros

// Log a message!
#define OKLOG(msg) do { \
    OK_OKLOG_OSTREAM << \
      OK_OKLOG_LOGTAG << " " << \
      OKLOG_FILEINFO() << " " << \
      msg << \
      std::endl; \
  } while (0)

// For simpler logging
#define OKLOG_NOFILE(msg) do { \
    OK_OKLOG_OSTREAM << \
      OK_OKLOG_LOGTAG << " " << \
      msg << \
      std::endl; \
  } while (0)

#if OK_OKLOG_ENABLE_DEBUG
#define OKLOG_DEBUG(msg) OKLOG(msg)
#else
#define OKLOG_DEBUG(msg) do { } while (0)
#endif

#if OK_OKLOG_ENABLE_ERROR
#define OKLOG_ERROR(msg) OKLOG(msg)
#else
#define OKLOG_ERROR(msg) do { } while (0)
#endif

// ==========================================================================
// Timing

#if OK_OKLOG_ENABLE_TIME

#include <sys/time.h>

namespace ok {

// Take a wall clock reading on instantiation
struct TimePoint {
  timeval time_point;
  TimePoint() {
    gettimeofday(&time_point, nullptr);
  }

  static uint64_t GetDurationMicros(const TimePoint &start, const TimePoint &end) {
    return
      (end.time_point.tv_sec * 1000000 + end.time_point.tv_usec) -
      (start.time_point.tv_sec * 1000000 + start.time_point.tv_usec);
  }

  static double GetDurationSeconds(const TimePoint &start, const TimePoint &end) {
    return double(1e-6) * GetDurationMicros(start, end);
  }
};

} /* namespace ok */

#ifndef OK_OKLOG_TIME_MESSAGE
#define OK_OKLOG_TIME_MESSAGE(elapsed_usec) \
  "[Elapsed: " << double(elapsed_usec * 1e-3) << \
  "ms (" << double(1000000. / elapsed_usec) << "Hz)]"
#endif

#define OKLOG_ELAPSED(start, msg) do { \
  ::ok::TimePoint end; \
  const uint64_t elapsed_usec = ::ok::TimePoint::GetDurationMicros(start, end); \
  OKLOG(msg << " " << OK_OKLOG_TIME_MESSAGE(elapsed_usec)); \
} while (0)

#else

namespace ok { struct TimePoint { TimePoint() { } }; }
#define OKLOG_ELAPSED(start, msg) do { } while (0)

#endif



#else

// ==========================================================================
// When logging is off, all statements just resolve to no-ops

#define OKLOG(msg) do { } while (0)
#define OKLOG_DEBUG(msg) do { } while (0)
#define OKLOG_ERROR(msg) do { } while (0)
#define OKLOG_NOFILE(msg) do { } while (0)

namespace ok { struct TimePoint { TimePoint() { } }; }
#define OKLOG_ELAPSED(start, msg) do { } while (0)


#endif

#endif /* OK_OKLOG_HPP_ */
