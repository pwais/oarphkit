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

#include "ok/fli/Core/Functors/Visitor.hpp"

using namespace ok;
using namespace ok::fli;

TEST(OKFLiVisitorTest, TestBasic) {

  struct State { int x; };

  class V : public Visitor<State> {
  public:
    int x;

    virtual void Call(State &s) override {
      x = s.x;
      s.x += 1;
    }

    void TestDatumCall(State &s) {
      Visitor::Call(rt_datumptr::Reference(&s));
    }
  };

  State state;
  state.x = 0;

  V v;
  v.x = -1;

  v.TestDatumCall(state);
  EXPECT_EQ(0, v.x);
  EXPECT_EQ(1, state.x);

  v.TestDatumCall(state);
  EXPECT_EQ(1, v.x);
  EXPECT_EQ(2, state.x);

  v.TestDatumCall(state);
  EXPECT_EQ(2, v.x);
  EXPECT_EQ(3, state.x);
}
