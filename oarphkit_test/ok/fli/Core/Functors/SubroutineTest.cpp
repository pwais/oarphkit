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

#include "gtest/gtest.h"

#include "ok/fli/Core/Functors/Subroutine.hpp"

using namespace ok;
using namespace ok::fli;

TEST(OKFLiSubroutineTest, TestBasic) {
  class SubTest : public Subroutine {
  public:
    int x;
    virtual void Call() override { ++x; }

    void TestDatumCall() {
      Subroutine::Call(rt_datumptr());
    }
  };

  SubTest s;
  s.x = 0;

  s.TestDatumCall();
  EXPECT_EQ(1, s.x);

  s.TestDatumCall();
  EXPECT_EQ(2, s.x);

  s.TestDatumCall();
  EXPECT_EQ(3, s.x);
}

