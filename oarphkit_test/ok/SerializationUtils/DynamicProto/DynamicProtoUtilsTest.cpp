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

#include "ok/SerializationUtils/DynamicProto/DynamicProtoUtils.hpp"
#include "ok/SerializationUtils/PBUtils.hpp"
#include "ok_test/OKAssertCheckThrows.hpp"

using namespace ok;
using namespace ok_msg;

TEST(DynamicProtoUtilsTest, TestEmpty) {

  // Nota bene: this test also exercises stubs in DynamicProtoUtils-nopb.hpp;

  DynamicProto p;
  EXPECT_EQ("DynamicProto()", DynToString(p));
  EXPECT_FALSE(DynHasValue<bool>(p));
  EXPECT_FALSE(DynHasKey(p, "moof"));
  EXPECT_TRUE(DynGetArray<uint64_t>(p).IsEmpty());

  // NB: w/out asserts, these return 0/""/IArray()
  EXPECT_OKASSERT_THROW_OR_EQ(DynGetValue<bool>(p), 0);
  EXPECT_OKASSERT_THROW_OR_EQ(DynGetValue<uint64_t>(p), 0);
  EXPECT_OKASSERT_THROW_OR_EQ(DynGetValue<const std::string &>(p), "");
  EXPECT_OKASSERT_THROW_OR_EQ(DynGetDynForKey(p, "moof"), nullptr);

  // Using `OrDefault` utils
  EXPECT_EQ(DynGetValueOrDefault<bool>(p, true), true);
  EXPECT_EQ(DynGetValueOrDefault<uint64_t>(p, 7), 7);
  EXPECT_EQ(DynGetValueOrDefault<const std::string &>(p, "moof"), "moof");

  EXPECT_OKASSERT_THROW_OR_EQ(
    DynGetValueForKey<uint64_t>(p, "moof"), 0);
  EXPECT_OKASSERT_THROW_OR_EQ(
    DynGetArrayForKey<uint64_t>(p, "moof").IsEmpty(), true);
}

#if OK_ENABLE_PROTOBUF

// Need protobuf to test DynamicProto with data

#define DEC_TEST_GETVALUE(TestName, TestVal, TestT, PBMethodName) \
  TEST(DynamicProtoUtilsTest, TestGetValue_##TestName) { \
    DynamicProto p; \
    EXPECT_FALSE(DynHasValue<TestT>(p)); \
    EXPECT_OKASSERT_THROW(DynGetValue<TestT>(p)); \
    p.set_##PBMethodName(TestVal); \
    EXPECT_EQ(TestVal, DynGetValue<TestT>(p)); \
  }

DEC_TEST_GETVALUE(string, "moof", const std::string &, vstr);
DEC_TEST_GETVALUE(sint, -5, int64_t, vint);
DEC_TEST_GETVALUE(uint, 5, uint64_t, vuint);
DEC_TEST_GETVALUE(boolean, false, bool, vbool);
DEC_TEST_GETVALUE(float, 5.0, float, vfloat);
DEC_TEST_GETVALUE(double, 5.0, double, vdouble);

#undef DEC_TEST_GETVALUE

#define DEC_TEST_SETVALUE(TestName, TestVal, TestT, PBMethodName) \
  TEST(DynamicProtoUtilsTest, TestSetValue_##TestName) { \
    DynamicProto p; \
    EXPECT_FALSE(DynHasValue<TestT>(p)); \
    EXPECT_OKASSERT_THROW(DynGetValue<TestT>(p)); \
    DynSetValue<TestT>(p, TestVal); \
    EXPECT_EQ(TestVal, DynGetValue<TestT>(p)); \
  }

DEC_TEST_SETVALUE(string, "moof", std::string, vstr);
DEC_TEST_SETVALUE(sint, -5, int64_t, vint);
DEC_TEST_SETVALUE(uint, 5, uint64_t, vuint);
DEC_TEST_SETVALUE(boolean, false, bool, vbool);
DEC_TEST_SETVALUE(float, 5.0, float, vfloat);
DEC_TEST_SETVALUE(double, 5.0, double, vdouble);

#undef DEC_TEST_SETVALUE

// We have to do bytes manually
TEST(DynamicProtoUtilsTest, TestValue_bytes) {

  DynamicProto p;
  EXPECT_FALSE(DynHasValue<ROByteIArray>(p));
  EXPECT_OKASSERT_THROW(DynGetValue<ROByteIArray>(p));
  EXPECT_FALSE(DynHasValue<ByteIArray>(p));
  EXPECT_OKASSERT_THROW(DynGetValue<ByteIArray>(p));

  auto arr = ByteIArray::Create(4);
  memcpy(arr.Data(), "moof", 4 * sizeof(byte));

  {
    DynamicProto p;
    p.set_vbytes(arr.Data(), arr.Size());
    auto decoded = DynGetValue<ROByteIArray>(p);
    EXPECT_EQ(arr.size(), decoded.size());
    for (size_t i = 0; i < arr.Size(); ++i) {
      EXPECT_EQ(arr.at(i), decoded.at(i));
    }
  }

  {
    DynamicProto p;
    p.set_vbytes(arr.Data(), arr.Size());
    auto decoded = DynGetValue<ByteIArray>(p);
    EXPECT_EQ(arr.size(), decoded.size());
    for (size_t i = 0; i < arr.Size(); ++i) {
      EXPECT_EQ(arr.at(i), decoded.at(i));
    }

    // Test expected mutability
    decoded.at(0) = 'l';
    EXPECT_EQ('l', p.vbytes().at(0));
  }

  {
    DynamicProto p;
    DynSetValue<ByteIArray>(p, arr);
    auto decoded = DynGetValue<ROByteIArray>(p);
    EXPECT_EQ(arr.size(), decoded.size());
    for (size_t i = 0; i < arr.Size(); ++i) {
      EXPECT_EQ(arr.at(i), decoded.at(i));
    }
  }

  {
    DynamicProto p;
    DynSetValue<ROByteIArray>(p, ROByteIArray::FascadeForContainer(arr));
    auto decoded = DynGetValue<ROByteIArray>(p);
    EXPECT_EQ(arr.size(), decoded.size());
    for (size_t i = 0; i < arr.Size(); ++i) {
      EXPECT_EQ(arr.at(i), decoded.at(i));
    }
  }
}

#define DEC_TEST_ARR(TestName, TestT, PBMethodName) \
  TEST(DynamicProtoUtilsTest, TestArray_##TestName) { \
    DynamicProto p; \
    EXPECT_TRUE(DynGetArray<TestT>(p).IsEmpty()); \
    std::vector<TestT> fixture; fixture.reserve(4); \
    fixture[0] = 1; fixture[1] = 2; fixture[2] = -1; fixture[3] = -2; \
    for (const auto &v : fixture) { \
      p.mutable_##PBMethodName()->Add(v); \
    } \
    IArray<TestT> arr = DynGetArray<TestT>(p); \
    EXPECT_EQ(fixture.size(), arr.size()); \
    for (size_t i = 0; i < arr.Size(); ++i) { \
      EXPECT_EQ(fixture.at(i), arr.at(i)); \
    } \
  }

DEC_TEST_ARR(sint, int64_t, vsint);
DEC_TEST_ARR(uint, uint64_t, vsuint);
DEC_TEST_ARR(sint32, int32_t, vsint32);
DEC_TEST_ARR(uint32, uint32_t, vsuint32);
DEC_TEST_ARR(float, float, vsfloat);
DEC_TEST_ARR(double, double, vsdouble);

#undef DEC_TEST_ARR

// We have to do string manually
TEST(DynamicProtoUtilsTest, TestArray_string) {
  DynamicProto p;
  EXPECT_TRUE(DynGetArray<const std::string *>(p).IsEmpty());
  auto arr = IArray<std::string>::Create(3);
  arr[0].assign("moof");
  arr[1].assign("oarph");
  arr[2].assign("ribbut");

  for (const auto &v : arr) {
    *p.mutable_vsstr()->Add() = v;
  }

  auto decoded = DynGetArray<const std::string *>(p);
  EXPECT_EQ(arr.size(), decoded.size());
  for (size_t i = 0; i < arr.size(); ++i) {
    EXPECT_EQ(arr.at(i), *decoded.at(i));
  }
}

#define DEC_TEST_SETARR(TestName, TestT) \
  TEST(DynamicProtoUtilsTest, TestSetArr_##TestName) { \
    DynamicProto p; \
    EXPECT_TRUE(DynGetArray<TestT>(p).IsEmpty()); \
    std::vector<TestT> fixture; fixture.reserve(4); \
    fixture[0] = 1; fixture[1] = 2; fixture[2] = -1; fixture[3] = -2; \
    DynSetArray<TestT>(p, IArray<TestT>::FascadeForContainer(fixture)); \
    IArray<TestT> arr = DynGetArray<TestT>(p); \
    EXPECT_EQ(fixture.size(), arr.size()); \
    for (size_t i = 0; i < arr.Size(); ++i) { \
      EXPECT_EQ(fixture.at(i), arr.at(i)); \
    } \
  }

DEC_TEST_SETARR(sint, int64_t);
DEC_TEST_SETARR(uint, uint64_t);
DEC_TEST_SETARR(sint32, int32_t);
DEC_TEST_SETARR(uint32, uint32_t);
DEC_TEST_SETARR(float, float);
DEC_TEST_SETARR(double, double);

#undef DEC_TEST_SETARR

// We have to do string manually
TEST(DynamicProtoUtilsTest, TestSetArr_string) {
  std::vector<std::string> strs(3);
  strs[0] = "moof";
  strs[1] = "oarph";
  strs[2] = "ribbut";

  {
    DynamicProto p;
    DynSetArray(p, IArray<std::string>::FascadeForContainer(strs));
    auto decoded = DynGetArray<const std::string *>(p);
    EXPECT_EQ(strs.size(), decoded.size());
    for (size_t i = 0; i < strs.size(); ++i) {
      EXPECT_EQ(strs.at(i), *decoded.at(i));
    }
  }

  {
    DynamicProto p;
    std::vector<std::string *> sptrs(3);
    sptrs[0] = &strs[0];
    sptrs[1] = &strs[1];
    sptrs[2] = &strs[2];
    DynSetArray(p, IArray<std::string *>::FascadeForContainer(sptrs));
    auto decoded = DynGetArray<const std::string *>(p);
    EXPECT_EQ(strs.size(), decoded.size());
    for (size_t i = 0; i < strs.size(); ++i) {
      EXPECT_EQ(strs.at(i), *decoded.at(i));
    }
  }
}



TEST(DynamicProtoUtilsTest, TestGetValueForKey) {
  DynamicProto p;
  EXPECT_FALSE(DynHasKey(p, "moof"));
  EXPECT_OKASSERT_THROW_OR_EQ(DynGetValueForKey<int64_t>(p, "moof"), 0);

  *p.mutable_k()->Add() = "moof";
  // Integrity error: no value!
  EXPECT_TRUE(DynHasKey(p, "moof"));
  EXPECT_OKASSERT_THROW_OR_EQ(DynGetValueForKey<int64_t>(p, "moof"), 0);

  p.mutable_v()->Add()->set_vint(4);
  EXPECT_EQ(4, DynGetValueForKey<int64_t>(p, "moof"));
}

TEST(DynamicProtoUtilsTest, TestSetValueForKey) {
  DynamicProto p;
  EXPECT_FALSE(DynHasKey(p, "moof"));
  EXPECT_OKASSERT_THROW_OR_EQ(DynGetValueForKey<int64_t>(p, "moof"), 0);

  DynSetValueForKey<int64_t>(p, "moof", 4);

  EXPECT_TRUE(DynHasKey(p, "moof"));
  EXPECT_EQ(4, DynGetValueForKey<int64_t>(p, "moof"));
}



TEST(DynamicProtoUtilsTest, TestGetArrayForKey) {
  DynamicProto p;
  EXPECT_FALSE(DynHasKey(p, "moof"));
  EXPECT_OKASSERT_THROW_OR_EQ(DynGetValueForKey<int64_t>(p, "moof"), 0);

  *p.mutable_k()->Add() = "moof";
  // Integrity error: no value!
  EXPECT_TRUE(DynHasKey(p, "moof"));
  EXPECT_OKASSERT_THROW_OR_EQ(
    DynGetArrayForKey<int64_t>(p, "moof").IsEmpty(), true);

  auto v = p.mutable_v()->Add();
  v->add_vsint(1);
  v->add_vsint(2);

  auto arr = DynGetArrayForKey<int64_t>(p, "moof");
  EXPECT_EQ(2, arr.Size());
  EXPECT_EQ(1, arr.at(0));
  EXPECT_EQ(2, arr.at(1));
}

TEST(DynamicProtoUtilsTest, TestSetArrayForKey) {
  DynamicProto p;
  EXPECT_FALSE(DynHasKey(p, "moof"));
  EXPECT_OKASSERT_THROW_OR_EQ(DynGetValueForKey<int64_t>(p, "moof"), 0);

  std::vector<int64_t> x = {1, 2};
  DynSetArrayForKey(p, "moof", IArray<int64_t>::FascadeForContainer(x));

  EXPECT_TRUE(DynHasKey(p, "moof"));
  auto arr = DynGetArrayForKey<int64_t>(p, "moof");
  EXPECT_EQ(2, arr.Size());
  EXPECT_EQ(1, arr.at(0));
  EXPECT_EQ(2, arr.at(1));
}



TEST(DynamicProtoUtilsTest, TestGetDynForKey) {
  DynamicProto p;
  EXPECT_FALSE(DynHasKey(p, "moof"));
  EXPECT_OKASSERT_THROW(DynGetDynForKey(p, "moof"));

  *p.mutable_k()->Add() = "moof";
  // Integrity error: no value!
  EXPECT_TRUE(DynHasKey(p, "moof"));
  EXPECT_OKASSERT_THROW(DynGetDynForKey(p, "moof"));

  auto v = p.mutable_v()->Add();
  v->add_k("k1");
  v->add_v()->set_vstr("v1");

  const auto *d = DynGetDynForKey(p, "moof");
  EXPECT_EQ("v1", DynGetValueForKey<const std::string &>(*d, "k1"));
}

TEST(DynamicProtoUtilsTest, TestSetDynForKeyPtr) {
  DynamicProto p;

  std::unique_ptr<DynamicProto> v1(new DynamicProto());
  DynSetValueForKey<uint64_t>(*v1, "submoof", 5);
  EXPECT_TRUE(DynHasKey(*v1, "submoof"));
  EXPECT_EQ(5, DynGetValueForKey<uint64_t>(*v1, "submoof"));

  EXPECT_FALSE(DynHasKey(p, "v1"));
  EXPECT_OKASSERT_THROW(DynGetDynForKey(p, "v1"));
  DynSetDynForKey(p, "v1", v1.release());

  EXPECT_TRUE(DynHasKey(p, "v1"));
  const auto *v1_got = DynGetDynForKey(p, "v1");
  EXPECT_TRUE(DynHasKey(*v1_got, "submoof"));
  EXPECT_EQ(5, DynGetValueForKey<uint64_t>(*v1_got, "submoof"));
}

TEST(DynamicProtoUtilsTest, TestSetDynForKeyRvalue) {
  DynamicProto p;

  DynamicProto v1;
  DynSetValueForKey<uint64_t>(v1, "submoof", 5);
  EXPECT_TRUE(DynHasKey(v1, "submoof"));
  EXPECT_EQ(5, DynGetValueForKey<uint64_t>(v1, "submoof"));

  EXPECT_FALSE(DynHasKey(p, "v1"));
  EXPECT_OKASSERT_THROW(DynGetDynForKey(p, "v1"));
  DynSetDynForKey(p, "v1", std::move(v1));

  EXPECT_TRUE(DynHasKey(p, "v1"));
  const auto *v1_got = DynGetDynForKey(p, "v1");
  EXPECT_TRUE(DynHasKey(*v1_got, "submoof"));
  EXPECT_EQ(5, DynGetValueForKey<uint64_t>(*v1_got, "submoof"));
}

TEST(DynamicProtoUtilsTest, TestSetDynForKeyCopy) {
  DynamicProto p;

  DynamicProto v1;
  DynSetValueForKey<uint64_t>(v1, "submoof", 5);
  EXPECT_TRUE(DynHasKey(v1, "submoof"));
  EXPECT_EQ(5, DynGetValueForKey<uint64_t>(v1, "submoof"));

  EXPECT_FALSE(DynHasKey(p, "v1"));
  EXPECT_OKASSERT_THROW(DynGetDynForKey(p, "v1"));
  DynSetDynForKey(p, "v1", v1);

  EXPECT_TRUE(DynHasKey(p, "v1"));
  const auto *v1_got = DynGetDynForKey(p, "v1");
  EXPECT_TRUE(DynHasKey(*v1_got, "submoof"));
  EXPECT_EQ(5, DynGetValueForKey<uint64_t>(*v1_got, "submoof"));
}



TEST(DynamicProtoUtilsTest, TestFromText) {
  DynamicProto p;
  *p.mutable_k()->Add() = "moof";
  p.mutable_v()->Add()->set_vstr("oarph");
  *p.mutable_k()->Add() = "moof2";
  p.mutable_v()->Add()->set_vint(4);
  *p.mutable_k()->Add() = "a_list";
  auto l = p.mutable_v()->Add()->mutable_vsfloat();
  l->Add(1);
  l->Add(-1);

  auto as_text = PBFactory::AsTextFormatString(p, true);
  const std::string kTypicalOutput =
      "k: \"moof\"\n"
      "k: \"moof2\"\n"
      "k: \"a_list\"\n"
      "v {\n  vStr: \"oarph\"\n}\n"
      "v {\n  vInt: 4\n}\n"
      "v {\n  vsFloat: 1\n  vsFloat: -1\n}\n";
  EXPECT_EQ(kTypicalOutput, as_text);

  // TextFormat is flexible enough for us to mix order of k and v
  // in human input
  const std::string kHumanInput =
      "k: \"moof\""
      "v { vStr: \"oarph\" }"

      "k: \"moof2\""
      "v { vInt: 4}"

      "k: \"a_list\""
      "v { vsFloat: 1 vsFloat: -1}";

  auto decoded = PBFactory::LoadFromString<DynamicProto>(kHumanInput);
  EXPECT_EQ("oarph", DynGetValueForKey<const std::string &>(*decoded, "moof"));
  EXPECT_EQ(4, DynGetValueForKey<int64_t>(*decoded, "moof2"));
  auto arr = DynGetArrayForKey<float>(*decoded, "a_list");
  EXPECT_EQ(2, arr.Size());
  EXPECT_EQ(1.f, arr[0]);
  EXPECT_EQ(-1.f, arr[1]);
}

#endif
