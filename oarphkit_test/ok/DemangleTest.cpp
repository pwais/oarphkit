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

#include <typeinfo>

#include "gtest/gtest.h"

#include "ok/Demangle.hpp"

using namespace ok;

#if __GNUC__

TEST(OKDemangleTest, TestBasic) {
  struct oarph { int x; };
  EXPECT_EQ("int", DemangledName(typeid(int)));
  EXPECT_EQ("float", DemangledName(typeid(float)));
  EXPECT_EQ("std::nullptr_t", DemangledName(typeid(nullptr)));
  EXPECT_EQ(
    "OKDemangleTest_TestBasic_Test::TestBody()::oarph",
    DemangledName(typeid(oarph)));
}

#endif
