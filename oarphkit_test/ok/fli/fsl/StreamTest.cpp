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

#include <list>

#include "ok/fli/Core/Functors/FuncWrapper.hpp"
#include "ok/fli/Core/Functors/Transform.hpp"
#include "ok/fli/fsl/Stream.hpp"

#include "ok_test/fli/OKFliUtils.hpp"

using namespace ok;
using namespace ok::fli;
using namespace ok::fli::fsl;
using namespace ok_test;

class ToTenSrc : public Source<int> {
public:

  int n;

  inline OutTypePtr Call() override {
    if (n < 10) {
      return MakeOwnedCopy(n++);
    } else {
      return NullTUPtr<int>();
    }
  }

  static Ptr Create() {
    return MakeOwned(new ToTenSrc());
  }

  virtual std::string RegistryAlias() const {
    return "fli_test::ToTenSrc";
  }

  ToTenSrc() : n(0) { }
  virtual ~ToTenSrc() { }
};

inline int AddOne(int x) { return x + 1; }
OK_FLI_DECLARE_FW(AddOneXform, AddOne, "fli_test::AddOne");

class ToListSnk : public Sink<int> {
public:

  std::list<int> ns;

  virtual void Call(ValType *arg=nullptr) {
    if (arg) { ns.push_back(*arg); }
  }

  typedef TUPtr<ToListSnk> Ptr;

  static Ptr Create() {
    return MakeOwned(new ToListSnk());
  }

  virtual std::string RegistryAlias() const {
    return "fli_test::ToListSnk";
  }

  virtual ~ToListSnk() { }
};

TEST(OKFLiStreamTest, TestBasic) {
  auto s = Stream::CreateEmpty();
  EXPECT_TRUE(s);
  EXPECT_NO_THROW(s->Call()); // empty stream
}

TEST(OKFLiStreamTest, TestSetSrc) {
  EXPECT_TRUE(Stream::CreateEmpty()->SetSource(ToTenSrc::Create()));
  EXPECT_FALSE(Stream::CreateEmpty()->SetSource(NullTUPtr<FunctorBase>()));
  EXPECT_CT_THROW_OR_FALSE(
    (Stream::CreateEmpty()->SetSource(Transform<int, int>::Create())));
}

TEST(OKFLiStreamTest, TestSetSnk) {
  EXPECT_TRUE(Stream::CreateEmpty()->SetSink(ToListSnk::Create()));
  EXPECT_FALSE(Stream::CreateEmpty()->SetSink(NullTUPtr<FunctorBase>()));
  EXPECT_CT_THROW_OR_FALSE(
    (Stream::CreateEmpty()->SetSink(Transform<int, int>::Create())));
}

TEST(OKFLiStreamTest, TestStream10) {
  auto s = Stream::CreateEmpty();
  EXPECT_TRUE(s->SetSource(ToTenSrc::Create()));

  auto snk = ToListSnk::Create();
  EXPECT_TRUE(s->SetSink(MakeUnOwned(snk.get())));

  EXPECT_NO_THROW(s->Call());

  EXPECT_EQ(10, snk->ns.size());
  int i = 0;
  for (const auto &entry : snk->ns) {
    EXPECT_EQ(i, entry);
    ++i;
  }
}

TEST(OKFLiStreamTest, TestPBEncoding) {
  env e;

  e.AssignFunctor("src", ToTenSrc::Create());

  auto snk = ToListSnk::Create();
  e.AssignFunctor("snk", MakeUnOwned(snk.get()));
    // We'll read from `snk` later, so don't let env own it

  static const std::string kStreamSpec =
      OK_STRINGIFY(
        alias: "fsl::Stream"
        [ok_msg.StreamInit.stream] {
          src {
            name: "src"
          }
          snk {
            name: "snk"
          }
        }
      );

  auto s = Stream::CreateEmpty();
  EXPECT_TRUE(s->SetSource(e.GetFunctor("src")));
  EXPECT_TRUE(s->SetSink(e.GetFunctor("snk")));

  e.GetRegistryRef().Register<Stream>();
  auto s_decoded =
      CheckFunctorEncoding(*s, kStreamSpec, false, &e);
  if (s_decoded) {
    EXPECT_NO_THROW(s_decoded->Call());
    EXPECT_EQ(10, snk->ns.size());
    int i = 0;
    for (const auto &entry : snk->ns) {
      EXPECT_EQ(i, entry);
      ++i;
    }
  }
}

TEST(OKFLiStreamTest, TestPBFromFliSpec) {
  static const std::string kStreamSpec =
      OK_STRINGIFY(
        alias: "fsl::Stream"
        name: "stream"
        [ok_msg.StreamInit.stream] {
          src {
            alias: "fli_test::ToTenSrc"
            name: "src"
          }
          snk {
            alias: "fli_test::ToListSnk"
            name: "snk"
          }
          proc {
            alias: "fli_test::AddOne"
            name: "addone"
          }
        }
      );

  env e;
  e.GetRegistryRef().Register<Stream>();
  e.GetRegistryRef().Register<ToTenSrc>();
  e.GetRegistryRef().Register<AddOneXform>();
  e.GetRegistryRef().Register<ToListSnk>();

  auto s = CheckFunctorEncodingFromFliSpec<Stream>(kStreamSpec, e);
  if (s) {
    EXPECT_TRUE(e.IsFunctor("stream"));
    EXPECT_EQ(s.get(), e.GetFunctor("stream").get());
    EXPECT_TRUE(e.IsFunctor("src"));
    EXPECT_TRUE(e.IsFunctor("addone"));
    EXPECT_TRUE(e.IsFunctor("snk"));
    EXPECT_EQ(4, e.GetFunctorToName().size());
  }
}

TEST(OKFLiStreamTest, TestPBEncodingWithProc) {
  /**
   * Similar to TestPBEncoding, but exercises `proc` 'syntactic sugar'.
   * We have to test this feature separately due to how
   * `CheckFunctorEncoding()` and standard PB tests require
   * an encoding spec serialize from and to the same text.
   */

  env e;

  e.AssignFunctor("src", ToTenSrc::Create());
  e.AssignFunctor("addone", MakeOwned(new AddOneXform()));

  auto snk = ToListSnk::Create();
  e.AssignFunctor("snk", MakeUnOwned(snk.get()));
    // We'll read from `snk` later, so don't let env own it

  static const std::string kStreamSpec =
      OK_STRINGIFY(
        alias: "fsl::Stream"
        [ok_msg.StreamInit.stream] {
          src {
            name: "src"
          }
          snk {
            name: "snk"
          }
          proc {
            name: "addone"
          }
        }
      );

  auto s = Stream::CreateEmpty();
  EXPECT_TRUE(s->SetSource(e.GetFunctor("src")));
  EXPECT_TRUE(s->SetSink(e.GetFunctor("snk")));

  #if OK_ENABLE_PROTOBUF

  e.GetRegistryRef().Register<Stream>();
  auto s_decoded = MakeOwned(new Stream());
  auto spec_decoded = PBFactory::LoadFromString<ok_msg::Func>(kStreamSpec);
  ASSERT_TRUE(s_decoded);
  ASSERT_TRUE(spec_decoded);
  auto func_attrs = FuncAttrsFromPB(*spec_decoded);
  EXPECT_TRUE(s_decoded->FromEncoded(e, func_attrs));

  EXPECT_NO_THROW(s_decoded->Call());
  EXPECT_EQ(10, snk->ns.size());
  int i = 0;
  for (const auto &entry : snk->ns) {
    EXPECT_EQ(i + 1, entry);
    ++i;
  }

  #endif /* OK_ENABLE_PROTOBUF */
}
