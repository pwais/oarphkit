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

#include "ok/fli/Core/Functors/Transform.hpp"
#include "ok/fli/fsl/Composition.hpp"

#include "ok_test/fli/OKFliUtils.hpp"

using namespace ok;
using namespace ok::fli;
using namespace ok::fli::detail;
using namespace ok::fli::fsl;
using namespace ok_test;

TEST(OKFliCompositionTest, TestBasic) {
  auto c = Composition::CreateEmpty();
  EXPECT_TRUE(c);
  EXPECT_EQ(nullptr, (c->Call<int, int>(nullptr)));
}

TEST(OKFliCompositionTest, TestCompileErrorCompositionAppend) {
  EXPECT_TRUE(
      (Composition::CreateEmpty()->Append(Transform<int, int>::Create())));

  auto c = Composition::CreateEmpty();
  c->Append(Transform<int, int>::Create());
  EXPECT_CT_THROW_OR_FALSE(
    (c->Append(Transform<float, float>::Create())));

  auto c2 = Composition::CreateEmpty();
  c2->Append(Transform<int, float>::Create());
  EXPECT_TRUE(
    (c2->Append(Transform<float, int>::Create())));
}

TEST(OKFliCompositionTest, TestCompileErrorCompositionPrepend) {
  EXPECT_TRUE(
      (Composition::CreateEmpty()->Prepend(Transform<int, int>::Create())));

  auto c = Composition::CreateEmpty();
  c->Prepend(Transform<int, int>::Create());
  EXPECT_CT_THROW_OR_FALSE(
    (c->Prepend(Transform<float, float>::Create())));

  auto c2 = Composition::CreateEmpty();
  c2->Prepend(Transform<int, float>::Create());
  EXPECT_TRUE(
    (c2->Prepend(Transform<float, int>::Create())));
}

class ComposeAddOneXform : public Transform<int, int> {
public:

  typedef TUPtr<Transform> Ptr;
  typedef TUPtr<int> OutTypePtr;

  static Ptr Create() { return MakeOwned(new ComposeAddOneXform()); }

  virtual std::string RegistryAlias() const {
    return "fli_test::ComposeAddOneXform";
  }

  TUPtr<int> Call(int *arg=nullptr) override {
    if (!arg) { return NullTUPtr<int>(); }
    return MakeOwnedCopy<int>((*arg) + 1);
  }
};

TEST(OKFliCompositionTest, TestNAdds) {
  auto c = Composition::CreateEmpty();
  EXPECT_TRUE(c->Append(ComposeAddOneXform::Create()));
  auto x = MakeOwnedCopy<int>(0);
  auto result = c->Call<int, int>(x.get());
  EXPECT_TRUE(result);
  EXPECT_EQ(1, *result);

  for (int i = 1; i < 10; ++i) {
    EXPECT_TRUE(c->Append(ComposeAddOneXform::Create()));
    result = c->Call<int, int>(x.get());
    EXPECT_EQ(i + 1, *result);
  }
}

TEST(OKFliCompositionTest, TestNPrepends) {
  auto c = Composition::CreateEmpty();
  EXPECT_TRUE(c->Prepend(ComposeAddOneXform::Create()));
  auto x = MakeOwnedCopy<int>(0);
  auto result = c->Call<int, int>(x.get());
  EXPECT_TRUE(result);
  EXPECT_EQ(1, *result);

  for (int i = 1; i < 10; ++i) {
    EXPECT_TRUE((c->Prepend(ComposeAddOneXform::Create())));
    result = c->Call<int, int>(x.get());
    EXPECT_EQ(i + 1, *result);
  }
}

TEST(OKFliCompositionTest, TestPrependAllEmpty) {
  std::list<FunctorBase::Ptr> empty;
  auto c = Composition::CreateEmpty();
  EXPECT_TRUE(c->PrependAll(std::move(empty)));
}

TEST(OKFliCompositionTest, TestPrependAllCantComposeWithHead) {
  std::list<FunctorBase::Ptr> cant_compose_with_head;
  cant_compose_with_head.push_back(Transform<int, float>::Create());

  auto c = Composition::CreateEmpty();
  c->Append(Transform<int, int>::Create());
  EXPECT_CT_THROW_OR_FALSE(c->PrependAll(cant_compose_with_head));
}

TEST(OKFliCompositionTest, TestPrependAllInnerCTFail) {
  std::list<FunctorBase::Ptr> inner_ct_fail;
  inner_ct_fail.emplace_back(Transform<int, int>::Create());
  inner_ct_fail.emplace_back(Transform<float, int>::Create());

  auto c = Composition::CreateEmpty();
  EXPECT_CT_THROW_OR_FALSE(c->PrependAll(inner_ct_fail));
}

TEST(OKFliCompositionTest, TestPrependAllValid) {
  std::list<FunctorBase::Ptr> valid;
  valid.emplace_back(ComposeAddOneXform::Create());
  valid.emplace_back(ComposeAddOneXform::Create());
  valid.emplace_back(ComposeAddOneXform::Create());

  auto c = Composition::CreateEmpty();
  EXPECT_TRUE(c->PrependAll(valid));

  auto x = MakeOwnedCopy<int>(0);
  auto result = c->Call<int, int>(x.get());
  EXPECT_TRUE(result);
  EXPECT_EQ(3, *result);

  for (int i = 2; i < 10; ++i) {
    std::list<FunctorBase::Ptr> validN;
    validN.emplace_back(ComposeAddOneXform::Create());
    validN.emplace_back(ComposeAddOneXform::Create());
    validN.emplace_back(ComposeAddOneXform::Create());
    EXPECT_TRUE(c->PrependAll(validN));
    result = c->Call<int, int>(x.get());
    EXPECT_EQ(i*3, *result);
  }
}



TEST(OKFliCompositionTest, TestAppendAllEmpty) {
  std::list<FunctorBase::Ptr> empty;
  auto c = Composition::CreateEmpty();
  EXPECT_TRUE(c->AppendAll(std::move(empty)));
}

TEST(OKFliCompositionTest, TestAppendAllCantComposeWithTail) {
  std::list<FunctorBase::Ptr> cant_compose_with_tail;
  cant_compose_with_tail.push_back(Transform<float, int>::Create());

  auto c = Composition::CreateEmpty();
  c->Append(Transform<int, int>::Create());
  EXPECT_CT_THROW_OR_FALSE(c->AppendAll(cant_compose_with_tail));
}

TEST(OKFliCompositionTest, TestAppendAllInnerCTFail) {
  std::list<FunctorBase::Ptr> inner_ct_fail;
  inner_ct_fail.emplace_back(Transform<int, int>::Create());
  inner_ct_fail.emplace_back(Transform<float, int>::Create());

  auto c = Composition::CreateEmpty();
  EXPECT_CT_THROW_OR_FALSE(c->AppendAll(inner_ct_fail));
}

TEST(OKFliCompositionTest, TestAppendAllValid) {
  std::list<FunctorBase::Ptr> valid;
  valid.emplace_back(ComposeAddOneXform::Create());
  valid.emplace_back(ComposeAddOneXform::Create());
  valid.emplace_back(ComposeAddOneXform::Create());

  auto c = Composition::CreateEmpty();
  EXPECT_TRUE(c->AppendAll(valid));

  auto x = MakeOwnedCopy<int>(0);
  auto result = c->Call<int, int>(x.get());
  EXPECT_TRUE(result);
  EXPECT_EQ(3, *result);

  for (int i = 2; i < 10; ++i) {
    std::list<FunctorBase::Ptr> validN;
    validN.emplace_back(ComposeAddOneXform::Create());
    validN.emplace_back(ComposeAddOneXform::Create());
    validN.emplace_back(ComposeAddOneXform::Create());
    EXPECT_TRUE(c->AppendAll(validN));
    result = c->Call<int, int>(x.get());
    EXPECT_EQ(i*3, *result);
  }
}



TEST(OKFliCompositionTest, TestPBEncoding) {
  env e;
  e.GetRegistryRef().Register<Composition>();
  e.GetRegistryRef().Register<ComposeAddOneXform>();

  e.AssignFunctor("addone", ComposeAddOneXform::Create());
  auto kCompSpec =
      OK_STRINGIFY(
        alias: "fsl::Composition"
        [ok_msg.CompositionInit.composition] {
          func {
            name: "addone"
          }
          func {
            name: "addone"
          }
        }
      );

  auto comp = Composition::CreateEmpty();
  comp->Append(e.GetFunctor("addone"));
  comp->Append(e.GetFunctor("addone"));

  auto comp_decoded =
      CheckFunctorEncoding(*comp, kCompSpec, false, &e);
  if (comp_decoded) {
    auto x = MakeOwnedCopy<int>(0);
    auto result = comp_decoded->Call<int, int>(x.get());
    EXPECT_TRUE(result);
    EXPECT_EQ(2, *result);
  }
}

TEST(OKFliCompositionTest, TestPBFromFliSpec) {
  auto kCompSpec =
      OK_STRINGIFY(
        alias: "fsl::Composition"
        name: "comp"
        [ok_msg.CompositionInit.composition] {
          func {
            alias: "fli_test::ComposeAddOneXform"
            name: "addone1"
          }
          func {
            alias: "fli_test::ComposeAddOneXform"
          }
        }
      );

  env e;
  e.GetRegistryRef().Register<Composition>();
  e.GetRegistryRef().Register<ComposeAddOneXform>();

  auto comp = CheckFunctorEncodingFromFliSpec<Composition>(kCompSpec, e);
  if (comp) {
    auto x = MakeOwnedCopy<int>(0);
    auto result = comp->Call<int, int>(x.get());
    EXPECT_TRUE(result);
    EXPECT_EQ(2, *result);

    EXPECT_TRUE(e.IsFunctor("comp"));
    EXPECT_EQ(comp.get(), e.GetFunctor("comp").get());
    EXPECT_TRUE(e.IsFunctor("addone1"));
    EXPECT_EQ(3, e.GetFunctorToName().size());
  }
}
