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

#include "ok/fli/Core/Core.hpp"

#include "ok_test/fli/OKFliUtils.hpp"

using namespace ok::fli;

TEST(OKFLiTypeCheckTest, TestFatal) {
  EXPECT_NO_THROW(
      OK_FLI_TYPECHECK(typeid(int), typeid(int), "test"));

#if OK_FLI_EXCEPTIONS_ENABLED
  EXPECT_THROW(
      OK_FLI_TYPECHECK(typeid(int), typeid(float), "test"),
      OK_FLI_TYPECHECK_ERROR_EXCEPTION_TYPE);
#else
  EXPECT_NO_THROW(OK_FLI_TYPECHECK(typeid(int), typeid(float), "test"));
#endif
}

TEST(OKFLiRTDatumTest, TestEmpty) {
  rt_datumptr d;
  EXPECT_EQ(nullptr, d.value);
#if OK_FLI_RUNTIME_TYPECHECK_ENABLED
  EXPECT_EQ(std::type_index(typeid(nullptr)), d.type);
#else
  EXPECT_EQ(sizeof(d), sizeof(d.value));
#endif

}

TEST(OKFLiRTDatumTest, TestIntegral) {
  auto xup = std::unique_ptr<int>(new int);
  *xup = 5;
  rt_datumptr d = rt_datumptr::Wrap(std::move(xup));
  EXPECT_TRUE(!xup); // datum now owns x
  EXPECT_TRUE(d.value);
  EXPECT_EQ(5, *d.Get<int>());
  EXPECT_EQ(5, d.GetRef<int>());
  EXPECT_RT_EQ(std::type_index(typeid(int)), d.type);
  EXPECT_RT_THROW(d.Get<float>());
}

TEST(OKFLiRTDatumTest, TestStruct) {

  struct oarph { int x; };

  auto oup = std::unique_ptr<oarph>(new oarph());
  oup->x = 5;
  rt_datumptr d = rt_datumptr::Wrap(std::move(oup));
  EXPECT_TRUE(!oup); // datum now owns o
  EXPECT_TRUE(d.value);
  EXPECT_EQ(5, d.Get<oarph>()->x);
  EXPECT_EQ(5, d.GetRef<oarph>().x);
  EXPECT_RT_EQ(std::type_index(typeid(oarph)), d.type);

  EXPECT_RT_THROW(d.Get<float>());
}



TEST(OKFLiCTDatumTest, TestEmpty) {
  ct_datumptr d;
  EXPECT_EQ(nullptr, d.value);
#if OK_FLI_COMPILETIME_TYPECHECK_ENABLED
  EXPECT_EQ(std::type_index(typeid(nullptr)), d.type);
#else
  EXPECT_EQ(sizeof(d), sizeof(d.value));
#endif

}

TEST(OKFLiCTDatumTest, TestIntegral) {
  auto xup = std::unique_ptr<int>(new int);
  *xup = 5;
  ct_datumptr d = ct_datumptr::Wrap(std::move(xup));
  EXPECT_TRUE(!xup); // datum now owns x
  EXPECT_TRUE(d.value);
  EXPECT_EQ(5, *d.Get<int>());
  EXPECT_EQ(5, d.GetRef<int>());
  EXPECT_CT_EQ(std::type_index(typeid(int)), d.type);
  EXPECT_CT_THROW(d.Get<float>());
}

TEST(OKFLiCTDatumTest, TestStruct) {
  struct oarph { int x; };
  auto oup = std::unique_ptr<oarph>(new oarph());
  oup->x = 5;
  ct_datumptr d = ct_datumptr::Wrap(std::move(oup));
  EXPECT_TRUE(!oup); // datum now owns o
  EXPECT_TRUE(d.value);
  EXPECT_EQ(5, d.Get<oarph>()->x);
  EXPECT_EQ(5, d.GetRef<oarph>().x);
  EXPECT_CT_EQ(std::type_index(typeid(oarph)), d.type);

  EXPECT_CT_THROW(d.Get<float>());
}



class TestAddOneFunctor : public FunctorBase {
public:
  TestAddOneFunctor() { }
  virtual ~TestAddOneFunctor() { }

  int AddOne(int x) {
    auto in = std::unique_ptr<int>(new int);
    *in = x;
    auto result_datum = this->Call(rt_datumptr::Wrap(std::move(in)));
    return result_datum.GetRef<int>();
  }

protected:

  virtual rt_datumptr Call(rt_datumptr in) override {
    int x = in.GetRef<int>();
    auto out = std::unique_ptr<int>(new int);
    *out = x + 1;
    return rt_datumptr::Wrap(std::move(out));
  }

};

TEST(OKFLiFunctorBase, TestBasic) {
  TestAddOneFunctor f;
  EXPECT_EQ(6, f.AddOne(5));
}


TEST(OKFLiFunctorBase, TestPB) {
  auto kMsg = "";
  auto f = MakeOwned(new FunctorBase());
  auto f_decoded =
      ok_test::CheckFunctorEncoding(*f, kMsg, true /* is pure */);
  #if OK_ENABLE_PROTOBUF
    EXPECT_TRUE(f_decoded);
  #endif
}
