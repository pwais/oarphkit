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

#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "ok/StringUtils.hpp"

using namespace ok;

typedef std::vector<int> intv;

TEST(StringUtilsTest, StartsWithStrTest) {
  EXPECT_TRUE(SStartsWith("", ""));
  EXPECT_TRUE(SStartsWith("a", "a"));
  EXPECT_TRUE(SStartsWith("ab", "a"));
  EXPECT_TRUE(SStartsWith("abc", "ab"));
  EXPECT_TRUE(SStartsWith("abc", "abc"));

  EXPECT_FALSE(SStartsWith("", "a"));
  EXPECT_FALSE(SStartsWith("a", "ab"));
  EXPECT_FALSE(SStartsWith("a", "b"));
  EXPECT_FALSE(SStartsWith("abc", "abd"));
  EXPECT_FALSE(SStartsWith("abcd", "abdc"));
}

TEST(StringUtilsTest, StartsWithVecTest) {
  EXPECT_TRUE(StartsWith(intv(), intv()));
  EXPECT_TRUE(StartsWith(intv({1}), intv({1})));
  EXPECT_TRUE(StartsWith(intv({1, 2}), intv({1})));
  EXPECT_TRUE(StartsWith(intv({1, 2}), intv({1, 2})));

  EXPECT_FALSE(StartsWith(intv(), intv({1})));
  EXPECT_FALSE(StartsWith(intv({1}), intv({1, 2})));
  EXPECT_FALSE(StartsWith(intv({1, 2, 4}), intv({1, 2, 3})));
  EXPECT_FALSE(StartsWith(intv({1, 4, 3}), intv({1, 2, 3})));
}

TEST(StringUtilsTest, EndsWithStrTest) {
  EXPECT_TRUE(SEndsWith("", ""));
  EXPECT_TRUE(SEndsWith("a", "a"));
  EXPECT_TRUE(SEndsWith("ba", "a"));
  EXPECT_TRUE(SEndsWith("cab", "ab"));
  EXPECT_TRUE(SEndsWith("abc", "abc"));

  EXPECT_FALSE(SEndsWith("", "a"));
  EXPECT_FALSE(SEndsWith("a", "ab"));
  EXPECT_FALSE(SEndsWith("a", "b"));
  EXPECT_FALSE(SEndsWith("aacd", "abd"));
  EXPECT_FALSE(SEndsWith("abcd", "abdc"));
}

TEST(StringUtilsTest, EndsWithVecTest) {
  EXPECT_TRUE(EndsWith(intv(), intv()));
  EXPECT_TRUE(EndsWith(intv({1}), intv({1})));
  EXPECT_TRUE(EndsWith(intv({2, 1}), intv({1})));
  EXPECT_TRUE(EndsWith(intv({2, 1}), intv({2, 1})));
  EXPECT_TRUE(EndsWith(intv({3, 2, 1}), intv({2, 1})));

  EXPECT_FALSE(EndsWith(intv(), intv({1})));
  EXPECT_FALSE(EndsWith(intv({1}), intv({1, 2})));
  EXPECT_FALSE(EndsWith(intv({1, 1, 2, 4}), intv({1, 2, 3})));
  EXPECT_FALSE(EndsWith(intv({1, 1, 2, 4}), intv({1, 1, 4})));
  EXPECT_FALSE(EndsWith(intv({1, 4, 3}), intv({1, 2, 3})));
}

TEST(StringUtilsTest, SReplaceAllInPlace) {

  #define CHECK(begin, expected, find, replace) do { \
    std::string s = begin; \
    SReplaceAllInPlace(s, find, replace); \
    EXPECT_EQ(s, expected); \
  } while (0)

  // Test empties and no-matches
  CHECK("",  "",   "", "");
  CHECK("",  "",   "a", "b");
  CHECK(" ", " ",  "a", "b");

  // Replace shorter with longer
  CHECK("a 1",        "a 1",           "12", "123");
  CHECK("a 12",       "a 123",         "12", "123");
  CHECK("a 12 ",      "a 123 ",        "12", "123");
  CHECK("a 12 12 12", "a 123 123 123", "12", "123");

  // Replace longer with shorter
  CHECK("a 1",        "a 1",           "123", "12");
  CHECK("a 123",      "a 12",          "123", "12");
  CHECK("a 123 ",     "a 12 ",         "123", "12");
  CHECK("a 123 123 123", "a 12 12 12", "123", "12");

  // Test removing a string
  CHECK("aaa", "",  "a", "");
  CHECK("a b a b",  " b  b",   "a", "");
  CHECK("a b a b ", "a  a  ",  "b", "");

  // Test find contains replace
  CHECK("a b a b ",   "a b a b ",   "a", "a");
  CHECK("a aa a aa ", "a a a a ",   "aa", "a");
}
