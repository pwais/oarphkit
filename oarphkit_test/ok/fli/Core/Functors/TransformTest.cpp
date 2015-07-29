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

#include "ok/fli/Core/Functors/Transform.hpp"

using namespace ok;
using namespace ok::fli;

TEST(OKFLiTransformTest, TestBasic) {
  auto d = Transform<int, int>::Create();
  EXPECT_TRUE(d);
  EXPECT_EQ(nullptr, d->Call(nullptr));
}

TEST(OKFLiTransformTest, TestAddOne) {
  class AddOneXform : public Transform<int, int> {
  public:
    AddOneXform() { }
    virtual ~AddOneXform() { }

    typedef TUPtr<int> OutTypePtr;

    virtual OutTypePtr Call(int *arg=nullptr) override {
      if (!arg) { return NullTUPtr<int>(); }
      int x = *arg;
      auto out = MakeOwned<int>(new int);
      *out = (x + 1);
      return std::move(out);
    }

    int TestDatumCall(int in) {
      std::unique_ptr<int> inp(new int);
      *inp = in;
      rt_datumptr outw = Transform::Call(rt_datumptr::Wrap(std::move(inp)));
      return outw.GetRef<int>();
    }
  };

  AddOneXform f;
  EXPECT_EQ(7, f.TestDatumCall(6));
  EXPECT_EQ(8, f.TestDatumCall(7));
  EXPECT_EQ(9, f.TestDatumCall(8));
}

