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

#include "ok/SV/SVMap.hpp"

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

TEST(SVMapTest, TestBasicInt) {
  OK_DECLARE_SV_ENTRY(IntEntry, int);

  SVMap m;
  EXPECT_TRUE(m.IsEmpty());

  EXPECT_FALSE(m.HasKey("k1"));
  EXPECT_THROW(m.GetValue<IntEntry>("k1"), std::out_of_range);
  EXPECT_THROW(m.GetValueRef<IntEntry>("k1"), std::out_of_range);
  EXPECT_THROW(m.GetValueAsString<IntEntry>("k1"), std::out_of_range);

  m.SetKey<IntEntry>("k1", 1);
  EXPECT_EQ(1, m.GetValue<IntEntry>("k1"));
  EXPECT_EQ(1, m.GetValueRef<IntEntry>("k1"));
  EXPECT_EQ(kSVMapOpaque, m.GetValueAsString<IntEntry>("k1"));

  m.GetValueRef<IntEntry>("k1") = 2;
  EXPECT_EQ(2, m.GetValueRef<IntEntry>("k1"));

  m.GetValueRef<IntEntry>("k1") += 1;
  EXPECT_EQ(3, m.GetValueRef<IntEntry>("k1"));
}

TEST(SVMapTest, TestBasicStr) {
  OK_DECLARE_SV_ENTRY(StrEntry, std::string);

  SVMap m;

  m.SetKey<StrEntry>("k1", "test");
  EXPECT_EQ("test", m.GetValue<StrEntry>("k1"));
  EXPECT_EQ("test", m.GetValueRef<StrEntry>("k1"));
  EXPECT_EQ(kSVMapOpaque, m.GetValueAsString<StrEntry>("k1"));

  m.GetValueRef<StrEntry>("k1") = "test2";
  EXPECT_EQ("test2", m.GetValueRef<StrEntry>("k1"));
}

TEST(SVMapTest, TestBasicIntPtr) {
  OK_DECLARE_SV_ENTRY(IntPtrEntry, int *);

  SVMap m;

  m.SetKey<IntPtrEntry>("k1", nullptr);
  EXPECT_EQ(nullptr, m.GetValue<IntPtrEntry>("k1"));
  EXPECT_EQ(nullptr, m.GetValueRef<IntPtrEntry>("k1"));
  EXPECT_EQ(kSVMapOpaque, m.GetValueAsString<IntPtrEntry>("k1"));

  int x = 5;
  m.SetKey<IntPtrEntry>("k1", &x);
  EXPECT_EQ(5, *m.GetValue<IntPtrEntry>("k1"));
  EXPECT_EQ(5, *m.GetValueRef<IntPtrEntry>("k1"));

  *m.GetValueRef<IntPtrEntry>("k1") = 2;
  EXPECT_EQ(2, *m.GetValue<IntPtrEntry>("k1"));

  *m.GetValueRef<IntPtrEntry>("k1") += 1;
  EXPECT_EQ(3, *m.GetValue<IntPtrEntry>("k1"));

  int y = 10;
  m.GetValueRef<IntPtrEntry>("k1") = &y;
  EXPECT_EQ(10, *m.GetValue<IntPtrEntry>("k1"));
}

TEST(SVMapTest, TestDefaultStringable) {
  OK_DECLARE_SV_DSTR_ENTRY(StrEntry, std::string);

  SVMap m;
  m.SetKey<StrEntry>("k1", "test");
  EXPECT_EQ("test", m.GetValueAsString<StrEntry>("k1"));

  m.GetValueRef<StrEntry>("k1") = "test2";
  EXPECT_EQ("test2", m.GetValueAsString<StrEntry>("k1"));
}

TEST(SVMapTest, TestUserStringable) {
  OK_DECLARE_SV_DSTR_ENTRY(BKey, B);

  SVMap m;
  B b;
  b.x = 5;
  m.SetKey<BKey>("k1", b);
  EXPECT_EQ("x=5", m.GetValueAsString<BKey>("k1"));
}

TEST(SVMapTest, TestGetKeys) {
  OK_DECLARE_SV_DSTR_ENTRY(StrEntry, std::string);

  SVMap m;
  EXPECT_SORTED_SEQUENCES_EQUAL(std::vector<std::string>(), m.GetKeys());

  m.SetKey<StrEntry>("k1", "test");
  m.SetKey<StrEntry>("k2", "test");

  std::vector<std::string> expected = {"k1", "k2"};
  EXPECT_SORTED_SEQUENCES_EQUAL(expected, m.GetKeys());
}

TEST(SVMapTest, TestGetType) {
  OK_DECLARE_SV_DSTR_ENTRY(StrEntry, std::string);

  SVMap m;
  EXPECT_THROW(m.GetValueType("k1"), std::out_of_range);

  m.SetKey<StrEntry>("k1", "test");
  EXPECT_EQ(m.GetValueType("k1"), StrEntry::EntryTypename());
}

TEST(SVMapTest, TestMoveTo) {
  OK_DECLARE_SV_DSTR_ENTRY(StrEntry, std::string);
  OK_DECLARE_SV_DSTR_ENTRY(IntEntry, int);

  SVMap m;
  EXPECT_FALSE(m.HasKey("s"));
  EXPECT_FALSE(m.HasKey("i"));

  {
    SVMap m2;
    m2.SetKey<StrEntry>("s", "moof");
    m2.SetKey<IntEntry>("i", 5);
    m2.MoveKeyTo("s", m);
  }

  EXPECT_TRUE(m.HasKey("s"));
  EXPECT_EQ("moof", m.GetValue<StrEntry>("s"));
  EXPECT_FALSE(m.HasKey("i"));

  {
    SVMap m2;
    m2.SetKey<StrEntry>("s", "cow");
    m2.SetKey<IntEntry>("i", 5);
    m2.MoveKeysTo(std::vector<std::string>({"s", "i", "no.existe"}), m);
  }

  EXPECT_FALSE(m.HasKey("no.existe"));
  EXPECT_TRUE(m.HasKey("s"));
  EXPECT_EQ("cow", m.GetValue<StrEntry>("s"));
  EXPECT_TRUE(m.HasKey("i"));
  EXPECT_EQ(5, m.GetValue<IntEntry>("i"));
}

TEST(SVMapTest, TestSplice) {
  OK_DECLARE_SV_DSTR_ENTRY(StrEntry, std::string);
  OK_DECLARE_SV_DSTR_ENTRY(IntEntry, int);

  SVMap m;
  EXPECT_FALSE(m.HasKey("s"));
  EXPECT_FALSE(m.HasKey("i"));
  EXPECT_TRUE(m.Splice(std::vector<std::string>()).IsEmpty());

  m.SetKey<StrEntry>("s", "moof");
  m.SetKey<IntEntry>("i", 5);

  EXPECT_TRUE(m.Splice(std::vector<std::string>()).IsEmpty());
  EXPECT_TRUE(m.Splice(std::vector<std::string>({"no.existe"})).IsEmpty());

  SVMap s1 = m.Splice(std::vector<std::string>({"s", "no.existe"}));
  EXPECT_FALSE(s1.HasKey("no.existe"));
  EXPECT_FALSE(m.HasKey("s"));
  EXPECT_TRUE(s1.HasKey("s"));
  EXPECT_EQ("moof", s1.GetValue<StrEntry>("s"));

  SVMap s2 = m.Splice(std::vector<std::string>({"s", "i"}));
  EXPECT_FALSE(s2.HasKey("s"));
  EXPECT_FALSE(m.HasKey("i"));
  EXPECT_TRUE(s2.HasKey("i"));
  EXPECT_EQ(5, s2.GetValue<IntEntry>("i"));

  EXPECT_TRUE(m.IsEmpty());
}

