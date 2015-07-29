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

#include <stdexcept>
#include <string>

#include "ok_test/SStreamSingleton.hpp"

#include "gtest/gtest.h"

using namespace ok_test;


// Configure OKAssert for testing
#define OK_OKASSERT_ENABLED 1
#define OKASSERT_CHECK_LOG(failmsg) do { \
  SStreamSingleton::GetSS() << failmsg; \
  } while(0)
#include "ok/OKAssert.hpp"


TEST(OKAssertTest, TestFatal) {
  EXPECT_THROW(OKASSERT_FATAL(false, "test"), std::runtime_error);
}

TEST(OKAssertTest, TestCheck) {
  bool foo = false;
  if (OKASSERT_CHECK(false, "test")) {
    foo = true;
  }
  EXPECT_FALSE(foo);

  static const std::string kExpectedPrefix = "Check failed: false";
  auto message = SStreamSingleton::GetStrAndReset();
  EXPECT_TRUE(message.size() >= kExpectedPrefix.size());
  EXPECT_EQ(kExpectedPrefix, message.substr(0, kExpectedPrefix.size()));
}
