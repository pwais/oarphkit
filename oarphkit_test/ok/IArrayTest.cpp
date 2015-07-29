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
#include <string>
#include <stdexcept>
#include <vector>

#include "gtest/gtest.h"

#include "ok/IArray.hpp"
#include "ok/StringUtils.hpp"
#include "ok_test/OKTAssertions.hpp"
#include "ok_test/OKAssertCheckThrows.hpp"

typedef ok::IArray<int> IntIArray;

template <typename ArrT>
void CheckArrEmpty(ArrT &arr) {
  EXPECT_EQ(nullptr, arr.Data());
  EXPECT_EQ(0, arr.Size());
  EXPECT_EQ(true, arr.IsEmpty());

  EXPECT_THROW(arr.at(0), std::out_of_range);
  EXPECT_OKASSERT_THROW(arr[0]);

  EXPECT_EQ(nullptr, arr.begin());
  EXPECT_EQ(nullptr, arr.end());
  EXPECT_OKASSERT_THROW(arr.front());
  EXPECT_OKASSERT_THROW(arr.back());

  EXPECT_EQ("IArray<int>(sz:0;data:null;owner:0;vals:  )", arr.ToString());
}

TEST(IntIArrayTest, TestEmpty) {
  IntIArray arr;
  CheckArrEmpty(arr);
}

TEST(IntIArrayTest, TestInit) {
  IntIArray arr = IntIArray::Create(10);
  EXPECT_EQ(10, arr.Size());
}

TEST(IntIArrayTest, TestInitZeroed) {
  IntIArray arr = IntIArray::CreateZeroed(10);
  EXPECT_EQ(10, arr.Size());
  for (size_t i = 0; i < arr.Size(); ++i) {
    EXPECT_EQ(0, arr.at(i));
  }

  EXPECT_TRUE(
    ok::SEndsWith(
      arr.ToString(),
      "owner:1;vals: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )"));
}

TEST(IntIArrayTest, TestMoveAssign) {
  IntIArray arr = IntIArray::Create(1);
  arr[0] = 5;
  IntIArray arr2;
  arr2 = std::move(arr);
  EXPECT_TRUE(arr.IsEmpty());
  EXPECT_FALSE(arr2.IsEmpty());
  EXPECT_EQ(5, arr2[0]);
}

TEST(IntIArrayTest, TestMoveCtor) {
  IntIArray arr = IntIArray::Create(1);
  arr[0] = 5;
  IntIArray arr2(std::move(arr));
  EXPECT_TRUE(arr.IsEmpty());
  EXPECT_FALSE(arr2.IsEmpty());
  EXPECT_EQ(5, arr2[0]);
}

TEST(IntIArrayTest, TestDeepCopy) {
  IntIArray arr = IntIArray::Create(2);
  arr[0] = 5;
  arr[1] = 6;
  IntIArray arr2 = arr.GetDeepCopy();
  EXPECT_EQ(arr.Size(), arr2.Size());
  EXPECT_EQ(5, arr2[0]);
  EXPECT_EQ(6, arr2[1]);
}

TEST(IntIArrayTest, TestReferencedContainer) {
  std::vector<int> x;
  x.push_back(1);

  IntIArray arr = IntIArray::FascadeForContainer(x);

  EXPECT_EQ(x.data(), arr.Data());
  EXPECT_EQ(1, arr.Size());
  EXPECT_EQ(false, arr.IsEmpty());

  EXPECT_EQ(x.at(0), arr.at(0));
  EXPECT_EQ(x[0], arr[0]);

  EXPECT_EQ(&x[0], arr.begin());
  EXPECT_EQ(&x[0] + x.size(), arr.end());
  EXPECT_EQ(x.front(), arr.front());
  EXPECT_EQ(x.back(), arr.back());

  EXPECT_SEQUENCES_EQUAL(x, arr);
  EXPECT_TRUE(ok::SStartsWith(arr.ToString(), "IArray<int>(sz:1;"));

  std::vector<int> y;
  y.push_back(1);
  EXPECT_SEQUENCES_EQUAL(y, arr);
}

TEST(IntIArrayTest, TestReferencedRange) {
  IntIArray arr = IntIArray::FascadeForRange(nullptr, nullptr);
  EXPECT_TRUE(arr.IsEmpty());

  std::vector<int> x;
  x.push_back(1);

  EXPECT_OKASSERT_THROW(
    IntIArray::FascadeForRange(&x[0] + x.size(), &x[0]));

  auto arr2 = IntIArray::FascadeForRange(&x[0], &x[0] + x.size());
  EXPECT_EQ(x.data(), arr2.Data());
  EXPECT_EQ(1, arr2.Size());
  EXPECT_EQ(false, arr2.IsEmpty());
}

TEST(IntIArrayTest, TestEmptyReferencedContainer) {
  std::vector<int> x;
  IntIArray arr = IntIArray::FascadeForContainer(x);

  CheckArrEmpty(arr);
}

TEST(IntIArrayTest, TestReferencedBuffer) {
  int x[1] = {1};
  IntIArray arr = IntIArray::FascadeForBuffer(x, 1);

  EXPECT_EQ(x, arr.Data());
  EXPECT_EQ(1, arr.Size());
  EXPECT_EQ(false, arr.IsEmpty());

  EXPECT_EQ(x[0], arr.at(0));
  EXPECT_EQ(x[0], arr[0]);

  EXPECT_EQ(&x[0], arr.begin());
  EXPECT_EQ(&x[0] + 1, arr.end());
  EXPECT_EQ(x[0], arr.front());
  EXPECT_EQ(x[0], arr.back());

  EXPECT_RANGES_EQUAL(x, x + 1, arr.begin(), arr.end());
  EXPECT_TRUE(ok::SStartsWith(arr.ToString(), "IArray<int>(sz:1;"));

  int y[1] = {1};
  EXPECT_RANGES_EQUAL(y, y + 1, arr.begin(), arr.end());
}

TEST(IntIArrayTest, TestReferencedNullBuffer) {
  int *x = nullptr;
  IntIArray arr = IntIArray::FascadeForBuffer(x, 0);

  CheckArrEmpty(arr);
}

TEST(IntIArrayTest, TestWrappedContainerBasic) {
  std::vector<int> x;
  x.push_back(1);

  IntIArray arr_rvalue = IntIArray::WrappingContainer(std::vector<int>(x));
  EXPECT_SEQUENCES_EQUAL(x, arr_rvalue);

  std::shared_ptr<std::vector<int>> xsp(new std::vector<int>(x));
  IntIArray arr_sptr = IntIArray::WrappingContainer(xsp);
  EXPECT_SEQUENCES_EQUAL(*xsp, arr_sptr);

  std::unique_ptr<std::vector<int>> xup(new std::vector<int>(x));
  IntIArray arr_uptr = IntIArray::WrappingContainer(std::move(xup));
  EXPECT_TRUE(!xup); // because arr now owns the temporary copy of x
  EXPECT_SEQUENCES_EQUAL(x, arr_uptr);
}

TEST(IntIArrayTest, TestWrappedBufferBasic) {
  int *x = new int[1];
  x[0] = 1;

  IntIArray::UPtr arrp =
      IntIArray::UPtr(
          new IntIArray(
              IntIArray::WrappingBuffer(x, 1, [&]{ delete[] x; x = nullptr;})));
  EXPECT_RANGES_EQUAL(x, x + 1, arrp->begin(), arrp->end());

  arrp = nullptr;
  EXPECT_EQ(nullptr, x);
}

TEST(IntIArrayTest, TestWrappedBufferBasic2) {
  int *x = new int[1];
  x[0] = 1;

  IntIArray arrp =
    IntIArray::WrappingBuffer(x, 1, [&]{ delete[] x; x = nullptr;});
      // NB: if `arrp` gets moved, we'll want the lambda to be [=] to *copy*
      // the pointer value
  EXPECT_FALSE(arrp.IsEmpty());
  EXPECT_RANGES_EQUAL(x, x + 1, arrp.begin(), arrp.end());

  arrp = IntIArray();
  EXPECT_TRUE(arrp.IsEmpty());
  EXPECT_EQ(nullptr, x);
}
