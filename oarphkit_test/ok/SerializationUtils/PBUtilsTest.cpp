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

#include "ok/Config.hpp"
#include "ok/SerializationUtils/PBUtils.hpp"

#if OK_ENABLE_PROTOBUF
// (otherwise, skip all tests)

#include <string>

#include "ok_test_msg/TestMessage.pb.h"

using namespace ok;
using namespace ok_test_msg;

typedef MUPtr<TestMessage> TestMessageUPtr;

static const std::string kTestMessageInTextFormat =
    "raw_str: \"oarph\"\n"
    "nums: 8";

TEST(PBFactoryTest, TestReadTextFormatString) {
  TestMessageUPtr m = PBFactory::LoadFromString<TestMessage>(kTestMessageInTextFormat);
  EXPECT_TRUE(m);
  EXPECT_TRUE(m->nums_size() > 0);
  EXPECT_EQ(8, m->nums(0));
  EXPECT_TRUE(m->has_raw_str());
  EXPECT_EQ("oarph", m->raw_str());
}

TEST(PBFactoryTest, TestWriteTextFormatString) {
  TestMessage m;
  m.set_raw_str("oarph");
  m.add_nums(8);
  auto s = PBFactory::AsTextFormatString(m, false);
  EXPECT_EQ("raw_str: \"oarph\" nums: 8 ", s);
}

TEST(PBFactoryTest, TestToFromArray) {
  TestMessage m;
  m.set_raw_str("oarph");
  m.add_nums(8);

  auto arr = PBFactory::AsArray(m);
  EXPECT_FALSE(arr.IsEmpty());
  EXPECT_EQ(m.ByteSize(), arr.Size());

  TestMessageUPtr dm = PBFactory::LoadFromContainer<TestMessage>(arr);
  EXPECT_TRUE(dm);
  EXPECT_TRUE(dm->nums_size() > 0);
  EXPECT_EQ(8, dm->nums(0));
  EXPECT_TRUE(dm->has_raw_str());
  EXPECT_EQ("oarph", dm->raw_str());
}

#endif
