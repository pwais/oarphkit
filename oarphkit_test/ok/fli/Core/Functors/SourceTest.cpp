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

#include "ok/fli/Core/Functors/Source.hpp"

using namespace ok;
using namespace ok::fli;

TEST(OKFLiSourceTest, TestBasic) {
  class SrcTest : public Source<int> {
  public:
    int x;
    typedef TUPtr<int> OutTypePtr;

    virtual OutTypePtr Call() override {
      ++x;
      auto p = MakeOwned<int>(new int);
      *p = x;
      return p;
    }

    int TestDatumCall() {
      auto datum = Source::Call(rt_datumptr());
      return datum.GetRef<int>();
    }
  };

  SrcTest s;
  s.x = 0;

  EXPECT_EQ(1, s.TestDatumCall());
  EXPECT_EQ(2, s.TestDatumCall());
  EXPECT_EQ(3, s.TestDatumCall());
}
