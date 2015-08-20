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

#include "ok_test/PBTestUtils.hpp"

using namespace ok;
using namespace ok::fli;

// = Core Tests ==============================================================

TEST(OKFLiEnvTest, TestDefault) {
  env e;
  EXPECT_TRUE(e.vars.HasKey("env.root_registry"));
  EXPECT_FALSE(e.vars.HasKey("env.anon_counter")); // lazily created
  EXPECT_FALSE(e.IsFunctor("moof"));
}

TEST(OKFLiEnvTest, TestAnonNames) {
  env e;

  EXPECT_FALSE(e.vars.HasKey("env.anon_counter"));
  EXPECT_EQ("test.0", e.CreateDistinctAnonName("test."));
  EXPECT_EQ("test.1", e.CreateDistinctAnonName("test."));
  EXPECT_TRUE(e.vars.HasKey("env.anon_counter"));
}

TEST(OKFLiEnvTest, TestFunctorsEmptyEnv) {
  env e;

  EXPECT_TRUE(e.GetFunctorToName().empty());
  EXPECT_FALSE(e.GetFunctor("moof"));
  EXPECT_FALSE(e.IsFunctor("moof"));
  EXPECT_TRUE(e.DecodeAllFunctors()); // Decoded nothing -> no errors
  EXPECT_TRUE(e.DecodeAllFunctors(true)); // Decoded nothing -> no errors
}

TEST(OKFLiEnvTest, TestFunctorsGetAndSet) {
  env e;
  auto f = MakeOwned(new FunctorBase());
  e.AssignFunctor("fun", std::move(f));
  EXPECT_TRUE(e.IsFunctor("fun"));

  auto f_fetched = e.GetFunctor("fun");
  EXPECT_TRUE(f_fetched);

  auto f_to_n = e.GetFunctorToName();
  EXPECT_TRUE(f_to_n.find(f_fetched.get()) != f_to_n.end());
  EXPECT_EQ(f_to_n[f_fetched.get()], "fun");

  EXPECT_TRUE(e.DecodeAllFunctors()); // Decoded nothing -> no errors
  EXPECT_TRUE(e.DecodeAllFunctors(true)); // Decoded nothing -> no errors
}

TEST(OKFLiEnvTest, TestClobberVars) {
  env e;

  // Clobbering an existing env var should work too
  OK_DECLARE_SV_ENTRY(IntEntry, int);
  e.vars.SetKey<IntEntry>("fun", 2);
  EXPECT_EQ(e.vars.GetValue<IntEntry>("fun"), 2);
  EXPECT_FALSE(e.IsFunctor("fun"));

  e.AssignFunctor("fun", MakeOwned(new FunctorBase()));
  EXPECT_TRUE(e.GetFunctor("fun"));
  EXPECT_TRUE(e.IsFunctor("fun"));
}





// = PB Encoding Tests =======================================================

TEST(OKFLiEnvTest, TestIPBBasic) {
  env e;
  EXPECT_TRUE(
    ok_test::CheckSerializesTo(
      e,
      OK_STRINGIFY(vars { })));
}

TEST(OKFLiEnvTest, TestIPBOneFunc) {
  static const std::string kEnvWithOneFunc =
    OK_STRINGIFY(
      vars {
        entry {
          key: "fun"
          wrapper_typename: "EnvFunctorEntry"
          [ok_msg.EnvFunctorEntryDataSVMapEntry_field] {
            func {
              alias: "fli::FunctorBase"
            }
            attrs {
            }
          }
        }
      });

  auto f = MakeOwned(new FunctorBase());
  env e;
  e.AssignFunctor("fun", std::move(f));

  #if OK_ENABLE_PROTOBUF
    EXPECT_TRUE(e.EncodeAllFunctors());
    EXPECT_TRUE(ok_test::CheckSerializesTo(e, kEnvWithOneFunc));
  #else
    EXPECT_FALSE(e.EncodeAllFunctors());
  #endif

  #if OK_ENABLE_PROTOBUF
    env e_decoded;
    e_decoded.GetRegistryRef().Register<FunctorBase>();
    EXPECT_TRUE(ok_test::CheckDecodesFrom(e_decoded, kEnvWithOneFunc));
    EXPECT_TRUE(e_decoded.DecodeAllFunctors());
    EXPECT_TRUE(e_decoded.GetFunctor("fun"));
  #else
    // Env will not decode; see IPBSerializableTest
  #endif
}

TEST(OKFLiEnvTest, TestEncodingPrefs) {
  static const std::string kEnvNoFunc = OK_STRINGIFY(vars { });

  auto f = MakeOwned(new FunctorBase());
  env e;
  e.AssignFunctor("fun", std::move(f));

  // Tell env to encode functors using some pref that doesn't exist
  OK_DECLARE_SV_ENTRY(InvalidEncodingPref, int);
  e.AddFunctorEncodingPref<InvalidEncodingPref>();
  EXPECT_TRUE(e.HasEncoding<InvalidEncodingPref>());

  // Encoding pass should fail
  EXPECT_FALSE(e.EncodeAllFunctors());

  #if OK_ENABLE_PROTOBUF
    // Env is also not in serializable state ...
    EXPECT_FALSE(ok_test::CheckSerializesTo(e, kEnvNoFunc));

    // ... until we remove the offending functor
    e.vars.ClearKey("fun");
    EXPECT_TRUE(ok_test::CheckSerializesTo(e, kEnvNoFunc));
  #endif
}



// = Functor Creation Tests ==================================================

void CheckCreateFunctor(env &e, SVStruct &&func_attrs) {
  e.GetRegistryRef().Register<FunctorBase>();

  #if OK_ENABLE_PROTOBUF
    EXPECT_TRUE(e.CreateFunctor(std::move(func_attrs)));
  #else
    EXPECT_FALSE(e.CreateFunctor(std::move(func_attrs)));
  #endif
}

TEST(OKFLiEnvTest, TestCreateFunctorFromBasic) {
  env e;

  SVStruct func_attrs;
  #if OK_ENABLE_PROTOBUF
  ok_msg::Func f_spec;
  f_spec.set_alias(FunctorBase().RegistryAlias());
  func_attrs.SetAttr<PBFuncAttr>(f_spec);
  #endif

  CheckCreateFunctor(e, std::move(func_attrs));

  #if OK_ENABLE_PROTOBUF
    // Env should have created a variable
    EXPECT_TRUE(e.vars.HasKey("anon.func.0"));
    EXPECT_TRUE(e.IsFunctor("anon.func.0"));
  #endif
}

TEST(OKFLiEnvTest, TestCreateFunctorFromWithVarname) {
  env e;

  SVStruct func_attrs;
  #if OK_ENABLE_PROTOBUF
    ok_msg::Func f_spec;
    f_spec.set_alias(FunctorBase().RegistryAlias());
    f_spec.set_name("fun");
    func_attrs.SetAttr<PBFuncAttr>(f_spec);
  #endif

  CheckCreateFunctor(e, std::move(func_attrs));

  #if OK_ENABLE_PROTOBUF
    // Env should have created a variable
    EXPECT_TRUE(e.vars.HasKey("fun"));
    EXPECT_TRUE(e.IsFunctor("fun"));
  #else
    EXPECT_FALSE(e.vars.HasKey("fun"));
    EXPECT_FALSE(e.IsFunctor("fun"));
  #endif

  // Env should now have "fun" set; lets reference it
  SVStruct func_attrs_fun2;
  #if OK_ENABLE_PROTOBUF
    ok_msg::Func f_spec2;
    f_spec2.set_name("fun");
    func_attrs_fun2.SetAttr<PBFuncAttr>(f_spec2);
  #endif

  CheckCreateFunctor(e, std::move(func_attrs_fun2));
}

TEST(OKFLiEnvTest, TestCreateFunctorReferencingVar) {
  env e;
  SVStruct func_attrs;

  #if OK_ENABLE_PROTOBUF
    ok_msg::Func f_spec;
    f_spec.set_name("fun");
    func_attrs.SetAttr<PBFuncAttr>(f_spec);
  #endif

  // No Functor at "fun" to get
  EXPECT_FALSE(e.vars.HasKey("fun"));
  EXPECT_FALSE(e.CreateFunctor(std::move(func_attrs)));

  // Now create an entry to reference
  e.AssignFunctor("fun", MakeOwned(new FunctorBase()));
  auto f = e.GetFunctor("fun");
  EXPECT_TRUE(f);
  EXPECT_TRUE(e.vars.HasKey("fun"));

  // Let's try that again
  SVStruct func_attrs2;
  #if OK_ENABLE_PROTOBUF
    ok_msg::Func f_spec2;
    f_spec2.set_name("fun");
    func_attrs2.SetAttr<PBFuncAttr>(f_spec2);

    auto fetched = e.CreateFunctor(std::move(func_attrs2));
    EXPECT_TRUE(fetched);
    EXPECT_EQ(fetched.get(), f.get());
  #else
    EXPECT_FALSE(e.CreateFunctor(std::move(func_attrs2)));
  #endif
}

TEST(OKFLiEnvTest, TestCreateFunctorErrorsInvalidAlias) {
  env e;
  SVStruct func_attrs;

  #if OK_ENABLE_PROTOBUF
    ok_msg::Func f_spec;
    f_spec.set_alias("no.existe");
    f_spec.set_name("fun");
    func_attrs.SetAttr<PBFuncAttr>(f_spec);
  #endif

  EXPECT_FALSE(e.CreateFunctor(std::move(func_attrs)));
  EXPECT_FALSE(e.vars.HasKey("fun"));
}

TEST(OKFLiEnvTest, TestNoMain) {
  env e;
  EXPECT_FALSE(e.HasMain());
  EXPECT_FALSE(e.RunMain());
  EXPECT_FALSE(e.GetMain());
  EXPECT_FALSE(e.SetMain(NullTUPtr<FunctorBase>()));
  EXPECT_FALSE(e.SetMain("no.existe"));
}

TEST(OKFLiEnvTest, TestSetMainStr) {
  env e;
  e.AssignFunctor("fun", MakeOwned(new FunctorBase()));

  EXPECT_TRUE(e.SetMain(e.GetFunctor("fun")));
  EXPECT_TRUE(e.RunMain());
  EXPECT_TRUE(e.GetMain());
  EXPECT_TRUE(e.HasMain());

  e.ClearMain();
  EXPECT_FALSE(e.RunMain());
  EXPECT_FALSE(e.GetMain());
  EXPECT_FALSE(e.HasMain());
}

TEST(OKFLiEnvTest, TestSetMainPtr) {
  env e;
  e.AssignFunctor("fun", MakeOwned(new FunctorBase()));

  EXPECT_TRUE(e.SetMain("fun"));
  EXPECT_TRUE(e.RunMain());
  EXPECT_TRUE(e.GetMain());

  e.ClearMain();
  EXPECT_FALSE(e.RunMain());
  EXPECT_FALSE(e.GetMain());
}

// = Multi-Env ===============================================================

void CheckEnvClone(env &e, env &ec) {
  EXPECT_EQ(
    e.vars.HasKey("env.encoding_prefs"),
    ec.vars.HasKey("env.encoding_prefs"));
  if (e.vars.HasKey("env.encoding_prefs")) {
    EXPECT_EQ(
      e.vars.GetValue<EncodingPrefsFuncAttr>("env.encoding_prefs"),
      ec.vars.GetValue<EncodingPrefsFuncAttr>("env.encoding_prefs"));
  }
  if (e.GetMain()) {
    EXPECT_EQ(
      e.vars.GetValue<EnvMainAttr>("env.main"),
      ec.vars.GetValue<EnvMainAttr>("env.main"));
  }
  if (e.vars.HasKey("env.anon_counter")) {
    EXPECT_EQ(
      e.vars.GetValue<EnvAnonCounterAttr>("env.anon_counter").val.load(),
      ec.vars.GetValue<EnvAnonCounterAttr>("env.anon_counter").val.load());
  }
  EXPECT_EQ(
    e.GetRegistry().GetAllAliases(),
    ec.GetRegistry().GetAllAliases());
  for (const auto &varname : e.vars.GetKeys()) {
    if (!env::IsEnvVar(varname)) {
      EXPECT_FALSE(ec.vars.HasKey(varname));
    }
  }
}

TEST(OKFLiEnvTest, TestCloneEmpty) {
  env e;
  env e2 = e.EmptyClone();
  CheckEnvClone(e, e2);
}

TEST(OKFLiEnvTest, TestCloneEncodingPrefs) {
  env e;
  e.AddFunctorEncodingPref<PBFuncAttr>();
  env e2 = e.EmptyClone();
  CheckEnvClone(e, e2);
}

TEST(OKFLiEnvTest, TestCloneAnonCounter) {
  env e;
  e.CreateDistinctAnonName();
  e.CreateDistinctAnonName();
  e.CreateDistinctAnonName();
  env e2 = e.EmptyClone();
  CheckEnvClone(e, e2);
}

TEST(OKFLiEnvTest, TestCloneRegistry) {
  env e;
  e.GetRegistryRef().Register<FunctorBase>();
  env e2 = e.EmptyClone();
  CheckEnvClone(e, e2);
}

TEST(OKFLiEnvTest, TestCloneMain) {
  env e;
  e.AssignFunctor("fun", MakeOwned(new FunctorBase()));
  e.SetMain("fun");
  env e2 = e.EmptyClone();
  CheckEnvClone(e, e2);
}



TEST(OKFLiEnvTest, TestMoveEnvVars) {
  env e1, e2;
  e1.AddFunctorEncodingPref<PBFuncAttr>();
  e1.MoveVarTo("env.encoding_prefs", e2);
  EXPECT_TRUE(e1.vars.HasKey("env.encoding_prefs")); // Move will copy pref
  EXPECT_FALSE(e2.vars.HasKey("env.encoding_prefs")); // not movable
}

TEST(OKFLiEnvTest, TestMoveRegularVar) {
  env e1, e2;

  OK_DECLARE_SV_DSTR_ENTRY(StrEntry, std::string);
  e1.vars.SetKey<StrEntry>("moof", "fish");

  e1.MoveVarTo("foo", e2);
  EXPECT_FALSE(e1.vars.HasKey("foo"));
  EXPECT_FALSE(e2.vars.HasKey("foo"));

  e1.MoveVarTo("moof", e2);
  EXPECT_FALSE(e1.vars.HasKey("moof"));
  EXPECT_TRUE(e2.vars.HasKey("moof"));
  EXPECT_EQ("fish", e2.vars.GetValue<StrEntry>("moof"));
}

typedef std::vector<std::string> VS;
void CheckVarsImp(const VS &vs, env &e, bool missing) {
  for (const auto &v : vs) {
    if (missing) {
      EXPECT_FALSE(e.vars.HasKey(v));
    } else {
      EXPECT_TRUE(e.vars.HasKey(v));
    }
  }
}
void CheckHasVars(const VS &vs, env &e) { CheckVarsImp(vs, e, false); }
void CheckMissingVars(const VS &vs, env &e) { CheckVarsImp(vs, e, true); }

TEST(OKFLiEnvTest, TestMoveRegularVars) {
  OK_DECLARE_SV_DSTR_ENTRY(StrEntry, std::string);
  env e1, e2;

  e1.vars.SetKey<StrEntry>("x1", "fish1");
  e1.vars.SetKey<StrEntry>("x2", "fish2");
  e1.vars.SetKey<StrEntry>("x3", "fish3");

  e1.MoveVarsTo(VS(), e2);
  CheckHasVars(VS({"x1", "x2", "x3"}), e1);
  CheckMissingVars(VS({"x1", "x2", "x3"}), e2);

  e1.MoveVarsTo(VS({"no.existe"}), e2);
  CheckHasVars(VS({"x1", "x2", "x3"}), e1);
  CheckMissingVars(VS({"x1", "x2", "x3"}), e2);

  e1.MoveVarsTo(VS({"x1"}), e2);
  CheckHasVars(VS({"x2", "x3"}), e1);
  CheckMissingVars(VS({"x1"}), e1);
  CheckHasVars(VS({"x1"}), e2);
  CheckMissingVars(VS({"x2", "x3"}), e2);

  e1.MoveVarsTo(VS({"x2", "x3"}), e2);
  CheckMissingVars(VS({"x1", "x2", "x3"}), e1);
  CheckHasVars(VS({"x1", "x2", "x3"}), e2);

  EXPECT_EQ("fish1", e2.vars.GetValue<StrEntry>("x1"));
  EXPECT_EQ("fish2", e2.vars.GetValue<StrEntry>("x2"));
  EXPECT_EQ("fish3", e2.vars.GetValue<StrEntry>("x3"));
}

TEST(OKFLiEnvTest, TestSpliceCopiesEnvVars) {
  env e;
  e.AddFunctorEncodingPref<PBFuncAttr>();
  e.CreateDistinctAnonName();
  e.CreateDistinctAnonName();
  e.CreateDistinctAnonName();
  e.GetRegistryRef().Register<FunctorBase>();

  e.AssignFunctor("fun", MakeOwned(new FunctorBase()));
  EXPECT_TRUE(e.SetMain("fun"));

  env es = e.Splice(VS());
  CheckEnvClone(e, es);
}

TEST(OKFLiEnvTest, TestSpliceNormalVars) {
  OK_DECLARE_SV_DSTR_ENTRY(StrEntry, std::string);
  env e1;

  e1.vars.SetKey<StrEntry>("x1", "fish1");
  e1.vars.SetKey<StrEntry>("x2", "fish2");
  e1.vars.SetKey<StrEntry>("x3", "fish3");

  {
    env e2 = e1.Splice(VS());
    CheckHasVars(VS({"x1", "x2", "x3"}), e1);
    CheckMissingVars(VS({"x1", "x2", "x3"}), e2);
  }

  {
    env e2 = e1.Splice(VS({"no.existe"}));
    CheckHasVars(VS({"x1", "x2", "x3"}), e1);
    CheckMissingVars(VS({"x1", "x2", "x3"}), e2);
  }

  {
    env e2 = e1.Splice(VS({"x1"}));
    CheckHasVars(VS({"x2", "x3"}), e1);
    CheckMissingVars(VS({"x1"}), e1);
    CheckHasVars(VS({"x1"}), e2);
    CheckMissingVars(VS({"x2", "x3"}), e2);
  }
}

TEST(OKFLiEnvTest, TestUnionEmpty) {
  env e;
  e.Union(env()); // Smoke
}

TEST(OKFLiEnvTest, TestUnionEncodingPrefs) {
  env e;
  env e2;
  e2.AddFunctorEncodingPref<PBFuncAttr>();
  EXPECT_FALSE(e.vars.HasKey("env.encoding_prefs"));
  EXPECT_TRUE(e2.vars.HasKey("env.encoding_prefs"));

  e.Union(std::move(e2));
  EXPECT_TRUE(e.vars.HasKey("env.encoding_prefs"));
  const auto &enc =
      e.vars.GetValue<EncodingPrefsFuncAttr>("env.encoding_prefs");
  EXPECT_TRUE(enc.find(PBFuncAttr::EntryTypename()) != enc.end());
}

TEST(OKFLiEnvTest, TestUnionAnonCounter) {
  env e;
  env e2;
  e2.CreateDistinctAnonName();
  e2.CreateDistinctAnonName();
  e2.CreateDistinctAnonName();
  EXPECT_EQ("0", e.CreateDistinctAnonName());
  EXPECT_EQ("3", e2.CreateDistinctAnonName());

  // Union doesn't touch
  e.Union(std::move(e2));
  EXPECT_EQ("1", e.CreateDistinctAnonName());
}

TEST(OKFLiEnvTest, TestUnionRegistry) {
  env e;
  env e2;
  e2.GetRegistryRef().Register<FunctorBase>();
  EXPECT_FALSE(e.GetRegistry().IsRegistered(FunctorBase().RegistryAlias()));
  EXPECT_TRUE(e2.GetRegistry().IsRegistered(FunctorBase().RegistryAlias()));

  e.Union(std::move(e2));
  EXPECT_TRUE(e.GetRegistry().IsRegistered(FunctorBase().RegistryAlias()));
}

TEST(OKFLiEnvTest, TestUnionMain) {
  env e;
  env e2;

  e2.AssignFunctor("fun", MakeOwned(new FunctorBase()));
  EXPECT_TRUE(e2.SetMain("fun"));

  e.Union(std::move(e2));
  EXPECT_TRUE(e.GetMain());
}

TEST(OKFLiEnvTest, TestUnionNormalVars) {
  OK_DECLARE_SV_DSTR_ENTRY(StrEntry, std::string);
  env e1;

  e1.vars.SetKey<StrEntry>("x1", "fish1");
  e1.vars.SetKey<StrEntry>("x2", "fish2");
  e1.vars.SetKey<StrEntry>("x3", "fish3");

  env e2;
  CheckHasVars(VS({"x1", "x2", "x3"}), e1);
  CheckMissingVars(VS({"x1", "x2", "x3"}), e2);

  e2.Union(std::move(e1));
  CheckHasVars(VS({"x1", "x2", "x3"}), e2);
}
