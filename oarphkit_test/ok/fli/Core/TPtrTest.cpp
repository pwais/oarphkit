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

#include "ok/fli/Core/TPtr.hpp"

using namespace ok::fli;

TEST(TPtrTest, TestMakeOwnedUP) {
  std::unique_ptr<int> x(new int);
  *x = 5;
  auto tp = MakeOwned(std::move(x));
  EXPECT_TRUE(tp);
  EXPECT_EQ(5, *tp);

  tp.reset();
  EXPECT_FALSE(tp);
}

TEST(TPtrTest, TestMakeOwnedTP) {
  auto tp = MakeOwned(new int);
  EXPECT_TRUE(tp);

  *tp = 5;
  EXPECT_EQ(5, *tp);

  tp.reset();
  EXPECT_FALSE(tp);
}

TEST(TPtrTest, TestMakeOwnedArr) {
  auto tp = MakeOwnedArray(new int[2]);
  EXPECT_TRUE(tp);

  *tp = 5;
  EXPECT_EQ(5, *tp);

  tp.reset();
  EXPECT_FALSE(tp);
}

TEST(TPtrTest, TestMakeOwnedCopy) {
  auto tp = MakeOwnedCopy(5);
  EXPECT_TRUE(tp);
  EXPECT_EQ(5, *tp);

  tp.reset();
  EXPECT_FALSE(tp);
}

TEST(TPtrTest, TestMakeUnowned) {
  std::unique_ptr<int> x(new int);
  *x = 5;

  auto tp = MakeUnOwned(x.get());
  EXPECT_TRUE(tp);
  EXPECT_EQ(5, *tp);

  tp.reset();
  EXPECT_FALSE(tp);

  tp = MakeOwnedCopy(55);
  tp.reset();
  EXPECT_FALSE(tp);

  EXPECT_TRUE(x);
  EXPECT_EQ(5, *x);
  *x = 6;
  EXPECT_EQ(6, *x);
}

TEST(TPtrTest, TestMakeUnownedUpcast) {
  class Base {
  public:
    int x;
  };

  class Derived : public Base {
  public:
    int y;
  };

  auto dp = MakeOwned(new Derived());

  auto bp = MakeUnOwned(dp.get());

  TUPtr<Derived> upcasted = MakeUnOwned(bp);
  EXPECT_TRUE(upcasted);
  EXPECT_EQ(dp.get(), upcasted.get());
}

TEST(TPtrTest, TestNullTUPtr) {
  EXPECT_FALSE(NullTUPtr<int>());
  EXPECT_EQ(nullptr, NullTUPtr<int>());
}

TEST(TPtrTest, TestToShared) {
  auto tp = MakeOwnedCopy(5);
  EXPECT_TRUE(tp);
  EXPECT_EQ(5, *tp);

  auto sp = ToShared(std::move(tp));
  EXPECT_TRUE(sp);
  EXPECT_EQ(5, *sp);
  EXPECT_FALSE(tp);
}
