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
#include "ok/fli/fsl/While.hpp"

#include "ok_test/OKAssertCheckThrows.hpp"
#include "ok_test/fli/OKFliUtils.hpp"

using namespace ok;
using namespace ok::fli;
using namespace ok::fli::fsl;
using namespace ok_test;

TEST(OKFLiWhileTest, TestBasic) {
  auto w = While::CreateEmpty();
  EXPECT_TRUE(w);
  EXPECT_NO_THROW(w->Call()); // Empty while is valid
}

TEST(OKFLiWhileTest, TestSetCond) {
  EXPECT_TRUE(
    While::CreateEmpty()->SetCondition(While::FalseCondition::Create()));
  EXPECT_FALSE(While::CreateEmpty()->SetCondition(NullTUPtr<FunctorBase>()));
  EXPECT_CT_THROW_OR_FALSE(
    (While::CreateEmpty()->SetCondition(Transform<int, int>::Create())));
}

TEST(OKFLiWhileTest, TestSetBody) {
  EXPECT_TRUE(
    (While::CreateEmpty()->SetBody(Subroutine::Create())));
  EXPECT_FALSE(While::CreateEmpty()->SetBody(NullTUPtr<FunctorBase>()));
  EXPECT_CT_THROW_OR_FALSE(
    (While::CreateEmpty()->SetBody(Transform<int, int>::Create())));
}

class Counter : public Subroutine {
public:
  Counter() : n(0) { }
  virtual ~Counter() { }

  uint64_t n;

  static TUPtr<Counter> Create() {
    return MakeOwned(new Counter());
  }

  virtual std::string RegistryAlias() const {
    return "fli_test::Counter";
  }

  virtual void Call() override {
    ++n;
  }
};

TEST(OKFLiWhileTest, TestLoopN) {
  auto w = While::CreateEmpty();
  auto c = Counter::Create();
  EXPECT_TRUE(w->SetCondition(While::ForN::Create(10)));
  EXPECT_TRUE(w->SetBody(MakeUnOwned(c.get())));

  EXPECT_NO_THROW(w->Call());
  EXPECT_EQ(10, c->n);
}

TEST(OKFLiWhileTest, TestPBEncoding) {
  env e;

  auto counter = Counter::Create();
  e.AssignFunctor("counter", MakeUnOwned(counter.get()));
  e.AssignFunctor("condition", While::ForN::Create(10));
  static const std::string kWhileSpec =
      OK_STRINGIFY(
        alias: "fsl::While"
        [ok_msg.WhileInit.while_] {
          condition {
            name: "condition"
          }
          body {
            name: "counter"
          }
        }
      );

  auto w = While::CreateEmpty();
  EXPECT_TRUE(w->SetCondition(e.GetFunctor("condition")));
  EXPECT_TRUE(w->SetBody(e.GetFunctor("counter")));

  e.GetRegistryRef().Register<While>();
  auto w_decoded = CheckFunctorEncoding(*w, kWhileSpec, false, &e);
  if (w_decoded) {
    EXPECT_NO_THROW(w_decoded->Call());
    EXPECT_EQ(10, counter->n);
  }
}

TEST(OKFLiWhileTest, TestPBFromFliSpec) {
  static const std::string kWhileSpec =
      OK_STRINGIFY(
        alias: "fsl::While"
        name: "while"
        [ok_msg.WhileInit.while_] {
          condition {
            alias: "fsl::While::ForN"
            fli_dyn_params { k: "n" v { vUInt: 10 } }
            name: "condition"
          }
          body {
            alias: "fli_test::Counter"
            name: "counter"
          }
        }
      );

  env e;
  e.GetRegistryRef().Register<While>();
  e.GetRegistryRef().Register<While::ForN>();
  e.GetRegistryRef().Register<Counter>();

  auto w = CheckFunctorEncodingFromFliSpec<While>(kWhileSpec, e);
  if (w) {
    EXPECT_TRUE(e.IsFunctor("while"));
    EXPECT_EQ(w.get(), e.GetFunctor("while").get());

    EXPECT_TRUE(e.IsFunctor("condition"));
    EXPECT_TRUE(e.IsFunctor("counter"));
    EXPECT_EQ(3, e.GetFunctorToName().size());

    // This test also exercised decoding of ForN; let's test the
    // decoded ForN operates correctly
    Counter *counter = dynamic_cast<Counter *>(e.GetFunctor("counter").get());
    EXPECT_TRUE(counter);
    EXPECT_NO_THROW(w->Call());
    EXPECT_EQ(10, counter->n);
  }
}

