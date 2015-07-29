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

#include <algorithm>

#include "gtest/gtest.h"

#include "ok_test/SStreamSingleton.hpp"

using namespace ok_test;


// Configure OKLOG for testing
#define OK_OKLOG_ENABLED 1
#define OK_OKLOG_LOGTAG "OKLogTestTag"
#define OK_OKLOG_OSTREAM SStreamSingleton::GetSS()
#include "ok/OKLog.hpp"



TEST(OKLogTest, TestBasic) {
  OKLOG("test test test");
  auto message = SStreamSingleton::GetStrAndReset();

  static const std::string kExpectedPrefix = OK_OKLOG_LOGTAG;
  EXPECT_TRUE(message.size() >= kExpectedPrefix.size());
  EXPECT_EQ(kExpectedPrefix, message.substr(0, kExpectedPrefix.size()));

  static const std::string kExpectedSuffix = "test test test";
  EXPECT_TRUE(message.size() >= kExpectedSuffix.size());
  EXPECT_EQ(
    kExpectedSuffix,
    message.substr(
      message.size() - kExpectedSuffix.size() - 1,
      kExpectedSuffix.size()));
}

TEST(OKLogTest, TestTiming) {
  ok::TimePoint start;
  for (size_t i = 0; i < 100; ++i) { OKLOG("spam"); }
  OKLOG_ELAPSED(start, "test test test");
  auto message = SStreamSingleton::GetStrAndReset();

  EXPECT_TRUE(message.find("test test test [Elapsed: "));
  EXPECT_TRUE(message.find("Hz)]"));
}
