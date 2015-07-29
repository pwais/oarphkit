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

#include "ok/fli/Runtime/env.hpp"
#include "ok/fli/Runtime/EnvFunctorEntryWrapper.hpp"

#include "ok_test/PBTestUtils.hpp"

using namespace ok;
using namespace ok::fli;

using namespace ok_test;

TEST(FLiEnvFunctorEntryWrapperTest, TestBasic) {
  env e;
  EnvFunctorEntryWrapper entry;
  EXPECT_FALSE(entry.IsCreated());
  EXPECT_FALSE(entry.GetFunctor());
  EXPECT_FALSE(entry.CreateAndGetFunctor(e));
  EXPECT_FALSE(entry.CreateFunctor(e));
  EXPECT_FALSE(entry.EncodeFunctor(e));

  EXPECT_TRUE(entry.GetAttrs().IsEmpty());

  entry.SetFunctor(MakeOwned(new FunctorBase()));
  EXPECT_TRUE(entry.IsCreated());
  EXPECT_TRUE(entry.GetFunctor());
  EXPECT_TRUE(entry.CreateAndGetFunctor(e));
  EXPECT_TRUE(entry.CreateFunctor(e));

  #if OK_ENABLE_PROTOBUF
    EXPECT_TRUE(entry.EncodeFunctor(e));
  #else
    // Can't encode
    EXPECT_FALSE(entry.EncodeFunctor(e));
  #endif
}

TEST(FLiEnvFunctorEntryWrapperTest, TestPB) {
  #if OK_ENABLE_PROTOBUF

  const static std::string kPBText =
      OK_STRINGIFY(
        func {
          alias: "fli::FunctorBase"
        }
        attrs {
          entry {
            wrapper_typename: "IsLazilyCreatedFuncAttr"
            dyndata {
              vBool: false
            }
          }
        });

  EXPECT_TRUE(CheckPBSerializableTextFormat<EnvFunctorEntryWrapper>(kPBText));

  env e;
  e.GetRegistryRef().Register<FunctorBase>();

  EnvFunctorEntryWrapper entry;
  entry.SetFunctor(MakeOwned(new FunctorBase()));
  entry.GetAttrsRef().SetAttr<IsLazilyCreatedFuncAttr>(false);
  EXPECT_TRUE(entry.EncodeFunctor(e));

  EXPECT_TRUE(CheckSerializesTo(entry, kPBText));

  EnvFunctorEntryWrapper entry2;
  EXPECT_TRUE(CheckDecodesFrom(entry2, kPBText));
  EXPECT_TRUE(entry2.CreateFunctor(e));
  EXPECT_TRUE(entry.IsCreated());

  #endif
}
