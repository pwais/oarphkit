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
#include "ok/fli/Runtime/env.hpp"
#include "ok/fli/Runtime/Registry.hpp"

using namespace ok;
using namespace ok::fli;

class RegAddOneXform : public Transform<int, int> {
public:

  typedef TUPtr<RegAddOneXform> Ptr;
  typedef TUPtr<int> OutTypePtr;

  virtual std::string RegistryAlias() const { return "RegAddOneXform"; }

  static Ptr Create() {
    return MakeOwned(new RegAddOneXform());
  }

  TUPtr<int> Call(int *arg=nullptr) override {
    if (!arg) { return NullTUPtr<int>(); }
    return MakeOwnedCopy<int>((*arg) + 1);
  }
};

class RegAddNXform : public Transform<int, int> {
public:

  typedef TUPtr<RegAddNXform> Ptr;
  typedef TUPtr<int> OutTypePtr;

  virtual std::string RegistryAlias() const { return "RegAddNXform"; }

  static Ptr Create(int n) {
    auto p = MakeOwned(new RegAddNXform());
    p->n_ = n;
    return p;
  }

  TUPtr<int> Call(int *arg=nullptr) override {
    if (!arg) { return NullTUPtr<int>(); }
    return MakeOwnedCopy<int>((*arg) + n_);
  }

protected:
  int n_;
};

TEST(OKFliRegistryTest, TestBasic) {
  Registry r;
  r.SetName("TestReg");
  EXPECT_EQ("TestReg", r.GetName());
  EXPECT_EQ("(empty)", r.GetAllAliases());

  EXPECT_FALSE(r.IsRegistered("moof"));
  EXPECT_FALSE(r.CreateFunctor("moof"));

  r.Register<RegAddOneXform>();
  r.Register<RegAddNXform>();

  EXPECT_EQ("RegAddNXform,RegAddOneXform", r.GetAllAliases(","));

  auto f = r.CreateFunctor("RegAddNXform");
  EXPECT_TRUE(f);

  auto f2 = r.CreateFunctor("oarph");
  EXPECT_FALSE(f2);

  auto fn = r.CreateFunctor("RegAddOneXform");
  EXPECT_TRUE(fn);
  int x = 1;
  auto out = fn->Call(rt_datumptr::Reference(&x));
  EXPECT_EQ(2, out.GetRef<int>());
}

TEST(OKFliRegistryTest, TestUnion) {
  Registry r1;
  r1.SetName("TestReg1");
  r1.Register<RegAddOneXform>();

  Registry r2;
  r2.SetName("TestReg2");
  r2.Register<RegAddNXform>();

  EXPECT_EQ("RegAddOneXform", r1.GetAllAliases(","));
  EXPECT_EQ("RegAddNXform", r2.GetAllAliases(","));

  EXPECT_TRUE(r1.UnionWith(r2));
  EXPECT_EQ("RegAddNXform,RegAddOneXform", r1.GetAllAliases(","));
  EXPECT_TRUE(r1.CreateFunctor("RegAddNXform"));
}

TEST(OKFliRegistryTest, TestUnionCollision) {
  class RegAddTwoXform : public Transform<int, int> {
  public:

    typedef TUPtr<RegAddTwoXform> Ptr;
    typedef TUPtr<int> OutTypePtr;

    virtual std::string RegistryAlias() const {
      return "RegAddOneXform";                  // Purposefully use wrong name
    }

    static Ptr Create() {
      return MakeOwned(new RegAddTwoXform());
    }

    TUPtr<int> Call(int *arg=nullptr) override {
      if (!arg) { return NullTUPtr<int>(); }
      return MakeOwnedCopy<int>((*arg) + 2);    // Add two, not one
    }
  };

  Registry r1;
  r1.SetName("TestReg1");
  r1.Register<RegAddOneXform>();

  Registry r2;
  r2.SetName("TestReg2");
  r2.Register<RegAddTwoXform>();

  // Alias collision!
  EXPECT_FALSE(r1.UnionWith(r2));

  EXPECT_EQ("RegAddOneXform", r1.GetAllAliases(","));
  EXPECT_EQ("RegAddOneXform", r2.GetAllAliases(",")); // Misnamed

  // But registries still work
  {
    auto fn = r1.CreateFunctor("RegAddOneXform");
    EXPECT_TRUE(fn);
    int x = 1;
    auto out = fn->Call(rt_datumptr::Reference(&x));
    EXPECT_EQ(2, out.GetRef<int>());
  }
  {
    auto fn = r2.CreateFunctor("RegAddOneXform");
    EXPECT_TRUE(fn);
    int x = 1;
    auto out = fn->Call(rt_datumptr::Reference(&x));
    EXPECT_EQ(3, out.GetRef<int>()); // Functor from r2 adds two, not one
  }
}

