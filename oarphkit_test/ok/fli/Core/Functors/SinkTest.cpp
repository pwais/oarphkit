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

#include "ok/fli/Core/Functors/Sink.hpp"

using namespace ok;
using namespace ok::fli;

TEST(OKFLiSinkTest, TestBasic) {
  class SnkTest : public Sink<int> {
  public:
    int n;

    virtual void Call(int *x=nullptr) override {
      EXPECT_TRUE(x);
      n += *x;
    }

    int TestDatumCall(int in) {
      std::unique_ptr<int> inp(new int);
      *inp = in;
      Sink::Call(rt_datumptr::Wrap(std::move(inp)));
      return n;
    }
  };

  SnkTest s;
  s.n = 0;

  EXPECT_EQ(1, s.TestDatumCall(1));
  EXPECT_EQ(3, s.TestDatumCall(2));
  EXPECT_EQ(6, s.TestDatumCall(3));
}
