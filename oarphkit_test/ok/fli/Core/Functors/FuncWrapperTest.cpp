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

#include "ok/Demangle.hpp"
#include "ok/fli/Core/Functors/FuncWrapper.hpp"
#include "ok/fli/fsl/Composition.hpp"

using namespace ok;
using namespace ok::fli;
using namespace ok::fli::fsl;

#define CHECK_SIG(FunctorT, ExpectedInT, ExpectedOutT) do { \
  FunctorT f; \
  FunctorBase *fb = &f; /* exercise virtualness */ \
  auto expected_in = std::type_index(typeid(ExpectedInT)); \
  auto expected_out = std::type_index(typeid(ExpectedOutT)); \
  EXPECT_EQ(DemangledName(expected_in), DemangledName(fb->InType())); \
  EXPECT_EQ(DemangledName(expected_out), DemangledName(fb->OutType())); \
} while (0)

void foo() { }

TEST(OKFLiFuncWrapperTest, TestMacro) {
  OK_FLI_DECLARE_FW(FooWrapper, foo, "fli::FooWrapper");

  FooWrapper w;
  EXPECT_EQ("fli::FooWrapper", w.RegistryAlias());
  CHECK_SIG(FooWrapper, Void, Void);
}

TEST(OKFLiFuncWrapperTest, TestSFINAETools_is_uptr) {
  static_assert(fw::is_uptr<std::unique_ptr<int>>::value, "");
  static_assert(!fw::is_uptr<std::shared_ptr<int>>::value, "");
  static_assert(!fw::is_uptr<int>::value, "");
}

TEST(OKFLiFuncWrapperTest, TestSFINAETools_remove_ptr_or_uptr) {
  static_assert(
    std::is_same<
      int,
      fw::remove_ptr_or_uptr<std::unique_ptr<int>>::type
        >::value, "");
  static_assert(
    std::is_same<
      int,
      fw::remove_ptr_or_uptr<int *>::type
        >::value, "");
  static_assert(
    std::is_same<
      std::shared_ptr<int>,
      fw::remove_ptr_or_uptr<std::shared_ptr<int>>::type
        >::value, "");
}

TEST(OKFLiFuncWrapperTest, TestSFINAETools_to_fli_type) {
  static_assert(
    std::is_same<
      int,
      fw::to_fli_void<int>::type
        >::value, "");
  static_assert(
    std::is_same<
      ok::fli::Void,
      fw::to_fli_void<void>::type
        >::value, "");
  static_assert(
    std::is_same<
      void *,
      fw::to_fli_void<void *>::type
        >::value, "");
}



namespace fwt {

struct movable {
  int x;

  movable() : x(0) { }

  movable(const movable &) = delete;
  movable &operator=(const movable &) = delete;
  movable(movable &&) = default;
  movable &operator=(movable &&) = default;
};

} /* namespace fwt */

#define DECL_FW(fun) \
  class FW : public FuncWrapper<decltype(fun)> { \
  public: \
    FW() { func_ = &fun; } \
    virtual ~FW() { } \
  }; \



namespace fwt {
int SubroutineFunc_x = 0;
void SubroutineFunc() { SubroutineFunc_x += 1; }
} /* namespace fwt */

TEST(OKFLiFuncWrapperTest, TestSubroutineFunc) {
  using namespace fwt;

  DECL_FW(SubroutineFunc);
  CHECK_SIG(FW, Void, Void);

  EXPECT_EQ(0, SubroutineFunc_x);
  FW f;
  f.Call();
  EXPECT_EQ(1, SubroutineFunc_x);
}



namespace fwt {
int SinkFun_x = 0;
void SinkFun(int x) { SinkFun_x += x; }
} /* namespace fwt */

TEST(OKFLiFuncWrapperTest, TestSinkFun) {
  using namespace fwt;

  DECL_FW(SinkFun);
  CHECK_SIG(FW, int, Void);

  EXPECT_EQ(0, SinkFun_x);
  FW f;
  int x = 5;
  f.Call(&x);
  EXPECT_EQ(5, SinkFun_x);
}



namespace fwt {
int SinkPtr_x = 0;
void SinkPtr(int *x) {
  EXPECT_TRUE(x);
  if (x) { SinkPtr_x += *x; }
}
} /* namespace fwt */

TEST(OKFLiFuncWrapperTest, TestSinkPtrFun) {
  using namespace fwt;

  DECL_FW(SinkPtr);
  CHECK_SIG(FW, int, Void);

  EXPECT_EQ(0, SinkPtr_x);
  FW f;
  int x = 5;
  f.Call(&x);
  EXPECT_EQ(5, SinkPtr_x);
}



namespace fwt {
int SinkUPtr_x = 0;
void SinkUPtr(std::unique_ptr<int> &&x) {
  EXPECT_TRUE(x);
  if (x) { SinkUPtr_x += *x; }
}
} /* namespace fwt */

TEST(OKFLiFuncWrapperTest, TestSinkUPtrFun) {
  using namespace fwt;

  DECL_FW(SinkUPtr);
  CHECK_SIG(FW, std::unique_ptr<int>, Void);

  EXPECT_EQ(0, SinkUPtr_x);
  FW f;
  std::unique_ptr<int> x(new int);
  *x = 5;
  f.Call(&x);
  EXPECT_EQ(5, SinkUPtr_x);
}



namespace fwt {
int SinkMv_x = 0;
void SinkMv(movable mv) {
  SinkMv_x += mv.x;
}
} /* namespace fwt */

TEST(OKFLiFuncWrapperTest, TestSinkMv) {
  using namespace fwt;

  DECL_FW(SinkMv);
  CHECK_SIG(FW, movable, Void);

  EXPECT_EQ(0, SinkMv_x);
  FW f;
  movable mv;
  mv.x = 5;
  f.Call(&mv);
  EXPECT_EQ(5, SinkMv_x);
}



namespace fwt {
int SourceFun() { return 5; }
} /* namespace fwt */

TEST(OKFLiFuncWrapperTest, TestSourceFun) {
  using namespace fwt;

  DECL_FW(SourceFun);
  CHECK_SIG(FW, Void, int);

  FW f;
  auto r = f.Call();
  EXPECT_TRUE(r);
  EXPECT_EQ(5, *r);
}



namespace fwt {
int *SourcePtr() {
  // Return an *unowned* pointer
  int *x = new int;
  *x = 5;
  return x;
}
} /* namespace fwt */

TEST(OKFLiFuncWrapperTest, TestSourcePtr) {
  using namespace fwt;

  DECL_FW(SourcePtr);
  CHECK_SIG(FW, Void, int);

  FW f;
  auto r = f.Call();
  EXPECT_TRUE(r);
  EXPECT_EQ(5, *r);
}



namespace fwt {
std::unique_ptr<int> SourceUPtr() {
  std::unique_ptr<int> x(new int);
  *x = 5;
  return x;
}
} /* namespace fwt */

TEST(OKFLiFuncWrapperTest, TestSourceUPtr) {
  using namespace fwt;

  DECL_FW(SourceUPtr);
  CHECK_SIG(FW, Void, int);

  FW f;
  auto r = f.Call();
  EXPECT_TRUE(r);
  EXPECT_EQ(5, *r);
}



namespace fwt {
movable SourceMv() {
  movable m;
  m.x = 5;
  return m;
}
} /* namespace fwt */

TEST(OKFLiFuncWrapperTest, TestSourceMv) {
  using namespace fwt;

  DECL_FW(SourceMv);
  CHECK_SIG(FW, Void, movable);

  FW f;
  auto r = f.Call();
  EXPECT_TRUE(r);
  EXPECT_EQ(5, r->x);
}



namespace fwt {
int Xform(int x) {
  return x + 1;
}
} /* namespace fwt */

TEST(OKFLiFuncWrapperTest, TestXform) {
  using namespace fwt;

  DECL_FW(Xform);
  CHECK_SIG(FW, int, int);

  FW f;
  int x = 5;
  auto r = f.Call(&x);
  EXPECT_TRUE(r);
  EXPECT_EQ(6, *r);
}



namespace fwt {
int *XformRetP(int x) {
  int *y = new int;
  *y = x + 1;
  return y;
}
} /* namespace fwt */

TEST(OKFLiFuncWrapperTest, TestXformRetP) {
  using namespace fwt;

  DECL_FW(XformRetP);
  CHECK_SIG(FW, int, int);

  FW f;
  int x = 5;
  auto r = f.Call(&x);
  EXPECT_TRUE(r);
  EXPECT_EQ(6, *r);
}



namespace fwt {
std::unique_ptr<int> XformRetUP(int x) {
  std::unique_ptr<int> y(new int);
  *y = x + 1;
  return y;
}
} /* namespace fwt */

TEST(OKFLiFuncWrapperTest, TestXformRetUP) {
  using namespace fwt;

  DECL_FW(XformRetUP);
  CHECK_SIG(FW, int, int);

  FW f;
  int x = 5;
  auto r = f.Call(&x);
  EXPECT_TRUE(r);
  EXPECT_EQ(6, *r);
}



namespace fwt {
int *XformPtr(int *x) {
  EXPECT_TRUE(x);
  int *y = new int;
  if (x) { *y = *x + 1; } else { *y = -500; }
  return y;
}
} /* namespace fwt */

TEST(OKFLiFuncWrapperTest, TestXformPtr) {
  using namespace fwt;

  DECL_FW(Xform);
  CHECK_SIG(FW, int, int);

  FW f;
  int x = 5;
  int *xp = &x;
  auto r = f.Call(xp);
  EXPECT_TRUE(r);
  EXPECT_EQ(6, *r);
}



namespace fwt {
movable XformMv(movable m) {
  m.x += 1;
  return m;
}
} /* namespace fwt */

TEST(OKFLiFuncWrapperTest, TestXformMv) {
  using namespace fwt;

  DECL_FW(XformMv);
  CHECK_SIG(FW, movable, movable);

  FW f;
  movable mv;
  mv.x = 1;
  auto r = f.Call(&mv);
  EXPECT_TRUE(r);
  EXPECT_EQ(2, r->x);
}



namespace fwt {
std::string AddK(const std::string &x) {
  return x + "k";
}
} /* namespace fwt */

TEST(OKFLiFuncWrapperTest, TestXformStr) {
  using namespace fwt;

  DECL_FW(AddK);
  CHECK_SIG(FW, std::string, std::string);

  FW f;
  auto input = std::string("test");
  auto r = f.Call(&input);
  EXPECT_TRUE(r);
  EXPECT_EQ("testk", *r);
}

#undef DECL_FW
#undef CHECK_SIG



#define DECL_FW_FOR_CLS(fun_cls) \
  OK_FLI_DECLARE_FW( \
    fun_cls, \
    fwt::fun_cls, \
    std::string("fwt::") + OK_STRINGIFY(fun_cls));

DECL_FW_FOR_CLS(SourceUPtr);
DECL_FW_FOR_CLS(SourcePtr);
DECL_FW_FOR_CLS(SourceFun);

DECL_FW_FOR_CLS(Xform);
DECL_FW_FOR_CLS(XformPtr);
DECL_FW_FOR_CLS(XformRetP);
DECL_FW_FOR_CLS(XformRetUP);

DECL_FW_FOR_CLS(SinkFun);
DECL_FW_FOR_CLS(SinkPtr);
// DECL_FW_FOR_CLS(SinkUPtr);
//    NB: FuncWrapper will never return a uptr, so user can't
//    compose with such a function

DECL_FW_FOR_CLS(SubroutineFunc);

#undef DECL_FW_FOR_CLS

#define APPEND_FW_CLS(clzz) do { \
  EXPECT_TRUE(comp->Append(MakeOwned<FunctorBase>(new clzz()))); \
} while(0);

TEST(OKFLiFuncWrapperTest, TestCompositionSources) {
  {
    auto comp = Composition::CreateEmpty();
    fwt::SinkFun_x = 0;

    APPEND_FW_CLS(SourceUPtr);  // Returns ptr to 5
    APPEND_FW_CLS(SinkFun);     // Adds to SinkFun_x, initially 0

    comp->Call<Void, Void>();
    EXPECT_EQ(5, fwt::SinkFun_x);
  }

  {
    auto comp = Composition::CreateEmpty();
    fwt::SinkFun_x = 0;

    APPEND_FW_CLS(SourcePtr);  // Returns ptr to 5
    APPEND_FW_CLS(SinkFun);     // Adds to SinkFun_x, initially 0

    comp->Call<Void, Void>();
    EXPECT_EQ(5, fwt::SinkFun_x);
  }

  {
    auto comp = Composition::CreateEmpty();
    fwt::SinkFun_x = 0;

    APPEND_FW_CLS(SourceFun);  // Returns 5
    APPEND_FW_CLS(SinkFun);     // Adds to SinkFun_x, initially 0

    comp->Call<Void, Void>();
    EXPECT_EQ(5, fwt::SinkFun_x);
  }
}

TEST(OKFLiFuncWrapperTest, TestCompositionSinkFun) {
  auto comp = Composition::CreateEmpty();
  fwt::SinkPtr_x = 0;

  APPEND_FW_CLS(SourceUPtr);  // Returns ptr to 5
  APPEND_FW_CLS(SinkPtr);     // Adds to SinkPtr_x, initially 0

  comp->Call<Void, Void>();
  EXPECT_EQ(5, fwt::SinkPtr_x);
}

TEST(OKFLiFuncWrapperTest, TestCompositionXformCompose) {
  auto comp = Composition::CreateEmpty();

  fwt::SinkFun_x = 0;

  // Thanks to FuncWrapper, we can compose a variety of xforms
  APPEND_FW_CLS(SourceUPtr);  // Returns ptr to 5
  APPEND_FW_CLS(Xform);       // Adds 1
  APPEND_FW_CLS(XformPtr);    // Adds 1
  APPEND_FW_CLS(XformRetP);   // Adds 1
  APPEND_FW_CLS(XformRetUP);  // Adds 1
  APPEND_FW_CLS(SinkFun);     // Adds to SinkFun_x, initially 0

  static const size_t kNumLoops = 5;
  for (size_t i = 0; i < kNumLoops; ++i) {
    comp->Call<Void, Void>();
  }
  EXPECT_EQ(9 * kNumLoops, fwt::SinkFun_x);
}

#undef APPEND_FW_CLS
