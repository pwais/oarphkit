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

#include <stdexcept>
#include <ostream>

#include "ok/SV/SVStruct.hpp"

#include "ok_test/OKTAssertions.hpp"
#include "ok_test/PBTestUtils.hpp"

namespace {

struct B { uint32_t x; };

std::ostream &operator<<(std::ostream &os, const B& obj) {
  os << "x=" << obj.x;
  return os;
}

}

using namespace ok;

TEST(SVStructTest, TestBasicInt) {
  OK_DECLARE_SV_ENTRY(IntKey, int);

  SVStruct m;
  EXPECT_TRUE(m.IsEmpty());

  EXPECT_FALSE(m.HasAttr<IntKey>());
  EXPECT_THROW(m.GetAttr<IntKey>(), std::out_of_range);
  EXPECT_THROW(m.GetAttrRef<IntKey>(), std::out_of_range);
  EXPECT_THROW(m.GetAttrAsString<IntKey>(), std::out_of_range);

  m.SetAttr<IntKey>(1);
  EXPECT_EQ(1, m.GetAttr<IntKey>());
  EXPECT_EQ(1, m.GetAttrRef<IntKey>());
  EXPECT_EQ(kSVMapOpaque, m.GetAttrAsString<IntKey>());

  m.GetAttrRef<IntKey>() = 2;
  EXPECT_EQ(2, m.GetAttrRef<IntKey>());

  m.GetAttrRef<IntKey>() += 1;
  EXPECT_EQ(3, m.GetAttrRef<IntKey>());
}

TEST(SVStructTest, TestBasicStr) {
  OK_DECLARE_SV_ENTRY(StrKey, std::string);

  SVStruct m;

  EXPECT_FALSE(m.HasAttr<StrKey>());
  EXPECT_THROW(m.GetAttr<StrKey>(), std::out_of_range);
  EXPECT_THROW(m.GetAttrRef<StrKey>(), std::out_of_range);
  EXPECT_THROW(m.GetAttrAsString<StrKey>(), std::out_of_range);

  m.SetAttr<StrKey>("test");
  EXPECT_EQ("test", m.GetAttr<StrKey>());
  EXPECT_EQ("test", m.GetAttrRef<StrKey>());
  EXPECT_EQ(kSVMapOpaque, m.GetAttrAsString<StrKey>());

  m.GetAttrRef<StrKey>() = "test2";
  EXPECT_EQ("test2", m.GetAttrRef<StrKey>());
}

TEST(SVStructTest, TestBasicIntPtr) {
  OK_DECLARE_SV_ENTRY(IntPtrKey, int *);

  SVStruct m;

  EXPECT_FALSE(m.HasAttr<IntPtrKey>());
  EXPECT_THROW(m.GetAttr<IntPtrKey>(), std::out_of_range);
  EXPECT_THROW(m.GetAttrRef<IntPtrKey>(), std::out_of_range);
  EXPECT_THROW(m.GetAttrAsString<IntPtrKey>(), std::out_of_range);

  m.SetAttr<IntPtrKey>(nullptr);
  EXPECT_EQ(nullptr, m.GetAttr<IntPtrKey>());
  EXPECT_EQ(nullptr, m.GetAttrRef<IntPtrKey>());
  EXPECT_EQ(kSVMapOpaque, m.GetAttrAsString<IntPtrKey>());

  int x = 5;
  m.SetAttr<IntPtrKey>(&x);
  EXPECT_EQ(5, *m.GetAttr<IntPtrKey>());
  EXPECT_EQ(5, *m.GetAttrRef<IntPtrKey>());

  *m.GetAttrRef<IntPtrKey>() = 2;
  EXPECT_EQ(2, *m.GetAttr<IntPtrKey>());

  *m.GetAttrRef<IntPtrKey>() += 1;
  EXPECT_EQ(3, *m.GetAttr<IntPtrKey>());

  int y = 10;
  m.GetAttrRef<IntPtrKey>() = &y;
  EXPECT_EQ(10, *m.GetAttr<IntPtrKey>());
}

TEST(SVStructTest, TestDefaultStringable) {
  OK_DECLARE_SV_DSTR_ENTRY(StrKey, std::string);

  SVStruct m;
  m.SetAttr<StrKey>("test");
  EXPECT_EQ("test", m.GetAttrAsString<StrKey>());

  m.GetAttrRef<StrKey>() = "test2";
  EXPECT_EQ("test2", m.GetAttrAsString<StrKey>());
}

TEST(SVStructTest, TestUserStringable) {
  OK_DECLARE_SV_DSTR_ENTRY(BKey, B);

  SVStruct m;
  B b;
  b.x = 5;
  m.SetAttr<BKey>(b);
  EXPECT_EQ("x=5", m.GetAttrAsString<BKey>());
}

TEST(SVStructTest, TestMoveTo) {
  OK_DECLARE_SV_DSTR_ENTRY(StrEntry, std::string);
  OK_DECLARE_SV_DSTR_ENTRY(IntEntry, int);

  SVStruct s;
  EXPECT_FALSE(s.HasAttr<StrEntry>());
  EXPECT_FALSE(s.HasAttr<IntEntry>());

  {
    SVStruct s2;
    s2.SetAttr<StrEntry>("moof");
    s2.SetAttr<IntEntry>(5);
    s2.MoveAttrTo<StrEntry>(s);
  }

  EXPECT_TRUE(s.HasAttr<StrEntry>());
  EXPECT_EQ("moof", s.GetAttr<StrEntry>());
  EXPECT_FALSE(s.HasAttr<IntEntry>());

  {
    SVStruct s2;
    s2.SetAttr<StrEntry>("cow");
    s2.SetAttr<IntEntry>(5);
    s2.MoveAttrTo<StrEntry>(s);
    s2.MoveAttrTo<IntEntry>(s);
  }

  EXPECT_TRUE(s.HasAttr<StrEntry>());
  EXPECT_EQ("cow", s.GetAttr<StrEntry>());
  EXPECT_TRUE(s.HasAttr<IntEntry>());
  EXPECT_EQ(5, s.GetAttr<IntEntry>());
}
