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

#include <vector>

#include "ok/fli/fsl/Range.hpp"
#include "ok/fli/fsl/Stream.hpp"

#include "ok_test/OKTAssertions.hpp"
#include "ok_test/PBTestUtils.hpp"

#if OK_ENABLE_PROTOBUF
#include "ok_msg/FLiSpec.pb.h"
#endif

using namespace ok;
using namespace ok::fli;
using namespace ok::fli::fsl;

TEST(OKFliRangeTest, TestEmpty) {
  EXPECT_FALSE(Range().Call());
  EXPECT_FALSE(Range(0, 0).Call());
  EXPECT_FALSE(Range(0, 1).Call());
  EXPECT_FALSE(Range(-1, -1).Call());

  // NB: Range protects against infinite ranges
  EXPECT_FALSE(Range(3, 0, -1).Call());
  EXPECT_FALSE(Range(3, 0, 0).Call());
}

TEST(OKFliRangeTest, TestThree) {
  Range r(3);
  EXPECT_EQ(*r.Call(), 0);
  EXPECT_EQ(*r.Call(), 1);
  EXPECT_EQ(*r.Call(), 2);
  EXPECT_FALSE(r.Call());
}

TEST(OKFliRangeTest, TestNegThree) {
  Range r(-3, 0, -1);
  EXPECT_EQ(*r.Call(), 0);
  EXPECT_EQ(*r.Call(), -1);
  EXPECT_EQ(*r.Call(), -2);
  EXPECT_FALSE(r.Call());
}

TEST(OKFliRangeTest, TestStep) {
  Range r(3, 0, 2);
  EXPECT_EQ(*r.Call(), 0);
  EXPECT_EQ(*r.Call(), 2);
  EXPECT_FALSE(r.Call());
}

TEST(OKFliRangeTest, TestStepOver) {
  Range r(3, 0, 3);
  EXPECT_EQ(*r.Call(), 0);
  EXPECT_FALSE(r.Call());
}

TEST(OKFliRangeTest, TestInStream) {
  class SnkTest : public Sink<int64_t> {
  public:
    std::vector<int64_t> nums;

    virtual void Call(int64_t *x=nullptr) override {
      EXPECT_TRUE(x);
      nums.push_back(*x);
    }
  };

  Range r(3);
  SnkTest snk;
  Stream stream;
  EXPECT_TRUE(stream.SetSource(MakeUnOwned(&r)));
  EXPECT_TRUE(stream.SetSink(MakeUnOwned(&snk)));
  stream.Call();

  EXPECT_FALSE(r.Call());
  std::vector<int64_t> expected = {0, 1, 2};
  EXPECT_SEQUENCES_EQUAL(expected, snk.nums);
}
