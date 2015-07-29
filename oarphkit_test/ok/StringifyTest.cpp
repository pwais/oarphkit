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

#include <memory>
#include <ostream>

#include "gtest/gtest.h"

#include "ok/Common.hpp"
#include "ok/Stringify.hpp"

using namespace ok;

TEST(StringifyTest, TestPOD) {
  EXPECT_EQ("1", Stringify(true));
  EXPECT_EQ("5", Stringify(5));
  EXPECT_EQ("5.5", Stringify(5.5));

  EXPECT_EQ("5", Stringify(byte(5)));
  EXPECT_EQ("9", Stringify(byte(9)));
  EXPECT_EQ("f", Stringify(byte(15)));
}

TEST(StringifyTest, TestStrings) {
  EXPECT_EQ("moof", Stringify("moof"));
  EXPECT_EQ("moof", Stringify(std::string("moof")));

  std::string s = "moof";
  EXPECT_EQ(s, Stringify(s));
}

TEST(StringifyTest, TestOpaque) {
  struct A { int x; };
  A a;
  EXPECT_EQ(strify::kOpaque, Stringify(a));
}

TEST(StringifyTest, OpaqueWithToString) {
  struct A {
    int x;
    std::string ToString() const {
      return "Moof!";
    }
  };
  A a;
  EXPECT_EQ("Moof!", Stringify(a));
}

TEST(StringifyTest, PtrToOpaqueWithToString) {
  struct A {
    int x;
    std::string ToString() const {
      return "Moof!";
    }
  };
  std::unique_ptr<A> a(new A());
  EXPECT_EQ("Moof!", Stringify(a));
}

struct B { int x; };
std::ostream &operator<<(std::ostream &os, const B& obj) {
  os << "x=" << obj.x;
  return os;
}

TEST(StringifyTest, TestStringableStruct) {
  B b;
  b.x = 5;
  EXPECT_EQ("x=5", Stringify(b));
}

TEST(StringifyTest, TestTruncation) {
  const std::string long_str = "I have 20 characters";
  EXPECT_EQ(long_str, Stringify(long_str));
  EXPECT_EQ(long_str, Stringify(long_str, -1));
  EXPECT_EQ(
      std::string("I have 20 ch..."),
      Stringify(long_str, 12));
}

TEST(StringifyTest, TestVectorPOD) {
  std::vector<int> x;
  EXPECT_EQ("vector(empty)", Stringify(x));

  x.push_back(5);
  EXPECT_EQ("vector(sz:1; 5)", Stringify(x));

  std::vector<int> y = {1, 2, 3, 4, 5};
  EXPECT_EQ("vector(sz:5; 1, 2, 3, 4, 5)", Stringify(y));
}

TEST(StringifyTest, TestVectorOfOpaque) {
  struct A { int x; };
  A a;
  std::vector<A> as;
  as.push_back(a);
  EXPECT_EQ("vector(sz:1; [Opaque to ok::Stringify])", Stringify(as));
}

TEST(StringifyTest, TestVectorOfStringableStruct) {
  B b;
  b.x = 5;
  std::vector<B> bs;
  bs.push_back(b);
  EXPECT_EQ("vector(sz:1; x=5)", Stringify(bs));
}

TEST(StringifyTest, TestVectorTruncation) {
  const int max_len = 20;
  std::vector<int> x;
  for (auto i = 0; i < max_len; ++i) {
    x.push_back(0);
  }

  EXPECT_EQ("vector(sz:20; 0, 0, ..)", Stringify(x, max_len));
}

TEST(StringifyTest, TestVectorTruncationWithStringableStruct) {
  const int max_len = 20;
  std::vector<B> bs;
  for (auto i = 0; i < max_len; ++i) {
    B b;
    b.x = 5;
    bs.push_back(b);
  }

  EXPECT_EQ("vector(sz:20; x=5, x..)", Stringify(bs, max_len));
}

TEST(StringifyTest, TestMapPOD) {
  std::unordered_map<int, float> x;
  EXPECT_EQ("unordered_map(empty)", Stringify(x));

  x[1] = 11.5;
  EXPECT_EQ("unordered_map(sz:1; 1:11.5)", Stringify(x));
}

TEST(StringifyTest, TestMapOfOpaque) {
  struct A { int x; };
  std::unordered_map<std::string, A> m;
  m["test"] = A();
  EXPECT_EQ("unordered_map(sz:1; test:" + strify::kOpaque + ")", Stringify(m));
}

TEST(StringifyTest, TestMapOfStringableStruct) {
  B b;
  b.x = 5;
  std::unordered_map<std::string, B> m;
  m["test"] = b;
  EXPECT_EQ("unordered_map(sz:1; test:x=5)", Stringify(m));
}

TEST(StringifyTest, TestMapTruncation) {
  const int max_len = 24;
  struct A { int x; };
  std::unordered_map<std::string, A> m;
  m["test"] = A();
  EXPECT_EQ("unordered_map(sz:1; test..)", Stringify(m, max_len));
}

TEST(StringifyTest, TestListPOD) {
  std::list<int> x;
  EXPECT_EQ("list(empty)", Stringify(x));

  x.push_back(5);
  EXPECT_EQ("list(5)", Stringify(x));

  std::list<int> y = {1, 2, 3, 4, 5};
  EXPECT_EQ("list(1, 2, 3, 4, 5)", Stringify(y));
}

TEST(StringifyTest, TestListOfStringableStruct) {
  B b;
  b.x = 5;
  std::list<B> l;
  l.push_back(b);
  EXPECT_EQ("list(x=5)", Stringify(l));
}

TEST(StringifyTest, TestListOfOpaque) {
  struct A { int x; };
  A a;
  std::list<A> as;
  as.push_back(a);
  EXPECT_EQ("list([Opaque to ok::Stringify])", Stringify(as));
}

TEST(StringifyTest, ListOfPtrToOpaqueWithToString) {
  struct A {
    int x;
    std::string ToString() const {
      return "Moof!";
    }
  };
  std::unique_ptr<A> a(new A());
  std::list<std::unique_ptr<A>> l;
  l.push_back(std::move(a));
  l.push_back(nullptr);
  EXPECT_EQ("list(Moof!, nullptr)", Stringify(l));
}

