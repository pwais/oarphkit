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
#ifndef OK_TEST_OKASSERTCHECKTHROWS_HPP_
#define OK_TEST_OKASSERTCHECKTHROWS_HPP_

#include "gtest/gtest.h"

#include "ok/Config.hpp"
#include "ok/OKAssert.hpp"

// If OKAssert is disabled, or is not intended to throw exceptions, then
// normal expressions shouldn't throw
#if !OK_OKASSERT_DISABLE_FATAL

  #if OKASSERT_EXCEPTIONS

    #define EXPECT_OKASSERT_THROW(exp) \
     EXPECT_THROW((exp), OKASSERT_EXCEPTION_TYPE)

    #define EXPECT_OKASSERT_THROW_OR_EQ(exp, val) \
      EXPECT_THROW((exp), OKASSERT_EXCEPTION_TYPE)

  #else

    #define EXPECT_OKASSERT_THROW(exp) EXPECT_NO_THROW((exp))

    #define EXPECT_OKASSERT_THROW_OR_EQ(exp, val) EXPECT_EQ((exp), val)

  #endif

#else /* OKASSERT FATAL disabled */

  #define EXPECT_OKASSERT_THROW(exp) EXPECT_NO_THROW((exp))

  #define EXPECT_OKASSERT_THROW_OR_EQ(exp, val) EXPECT_EQ((exp), val)

#endif

#endif /* OK_TEST_OKASSERTCHECKTHROWS_HPP_ */
