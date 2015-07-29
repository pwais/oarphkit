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
#include "ok/SerializationUtils/IPBSerializable.hpp"
#include "ok/SerializationUtils/PBUtils.hpp"
#include "ok/SV/SVMap.hpp"
#include "ok/SV/SVStruct.hpp"

#include "ok_test/OKTAssertions.hpp"
#include "ok_test/PBTestUtils.hpp"

#if OK_ENABLE_PROTOBUF
#include "ok_test_msg/SVMapPBTest.pb.h"
#else
namespace ok_test_msg { class SmallTestMessage; }
#endif

using namespace ok;

class TestClass : public IPBSerializable<ok_test_msg::SmallTestMessage> {
public:
  TestClass() { }
  explicit TestClass(const std::string &v) : val(v) { }
  virtual ~TestClass() { }

  std::string val;

  #if OK_ENABLE_PROTOBUF
  virtual bool FromPBMessage(MessageType &m) {
    val = m.vstr();
    return true;
  }

  virtual bool ToPBMessage(MessageType &m) {
    m.set_vstr(val);
    return true;
  }
  #endif

  std::string ToString() const {
    return "TestClass value: " + val;
  }
};

OK_DECLARE_SV_DSTRUL_IPB_ENTRY(
    TestClassEntry,
    TestClass,
    ok_test_msg::SmallTestMessageSVMapEntry_field);


#if OK_ENABLE_PROTOBUF // ====================================================

TEST(SVMapPBTest, TestPBSerialization) {
  SVMap m;
  m.SetKey<TestClassEntry>("k1", TestClass("test"));

  // Add a non-serializable entry
  OK_DECLARE_SV_DSTR_ENTRY(StrKey, std::string);
  m.SetKey<StrKey>("k2", "test2");

  // Sanity check
  EXPECT_EQ(m.GetValue<TestClassEntry>("k1").val, "test");
  EXPECT_EQ(m.GetValue<StrKey>("k2"), "test2");

  // Test serialization
  static std::string kPBTextEncoding =
    OK_STRINGIFY(
      entry {
        key: "k1"
        wrapper_typename: "TestClassEntry"
        [ok_test_msg.SmallTestMessageSVMapEntry_field] {
          vstr: "test"
        }
      });
  EXPECT_TRUE(ok_test::CheckSerializesTo(m, kPBTextEncoding));

  auto decoded_pbm =
      PBFactory::LoadFromString<ok_msg::SVMapData>(kPBTextEncoding);
  EXPECT_TRUE(decoded_pbm);

  SVMap decoded_m;
  decoded_m.RegisterEntry<TestClassEntry>();
  EXPECT_TRUE(decoded_m.FromPBMessage(*decoded_pbm));
  EXPECT_TRUE(decoded_m.HasKey("k1"));
  EXPECT_EQ(decoded_m.GetValue<TestClassEntry>("k1").val, "test");

  // k2 was not encoded
  EXPECT_FALSE(decoded_m.HasKey("k2"));

  // Try without registering; should end up empty.
  SVMap naive_m;
  EXPECT_TRUE(naive_m.FromPBMessage(*decoded_pbm));
  EXPECT_FALSE(naive_m.HasKey("k1"));

  // Exercise Registered Entry copying
  SVMap other_m;
  EXPECT_TRUE(other_m.FromPBMessage(*decoded_pbm));
  EXPECT_FALSE(other_m.HasKey("k1"));
  decoded_m.CopyRegisteredEntriesTo(other_m);
  EXPECT_TRUE(other_m.FromPBMessage(*decoded_pbm));
  EXPECT_TRUE(other_m.HasKey("k1"));
}

// = Test Dyn Serialization Features =============================
template <typename EntryT>
struct PBDynSerializationHarness {

  typedef typename EntryT::WrappedType WType;

  virtual WType CreateFixture() = 0;

  virtual std::string GetExpectedEncoding() { return ""; }

  virtual void CheckValue(const WType &v) { }

  void Run() {
    SVMap m;
    m.SetKey<EntryT>("v", CreateFixture());

    // Test serialization
    const std::string kPBTextEncoding = GetExpectedEncoding();
    EXPECT_TRUE(ok_test::CheckSerializesTo(m, kPBTextEncoding));

    auto decoded_pbm =
        PBFactory::LoadFromString<ok_msg::SVMapData>(kPBTextEncoding);
    EXPECT_TRUE(decoded_pbm);

    SVMap decoded_m;
    decoded_m.RegisterEntry<EntryT>();

    if (decoded_pbm) { EXPECT_TRUE(decoded_m.FromPBMessage(*decoded_pbm)); }
    EXPECT_TRUE(decoded_m.HasKey("v"));

    CheckValue(decoded_m.GetValue<EntryT>("v"));
  }
};

TEST(SVMapPBTest, TestPBDynSerializationInt) {
  OK_DECLARE_SV_DSTRUL_PBDYNDATA_ENTRY(IntKey, int64_t);

  struct Harness : public PBDynSerializationHarness<IntKey> {
    WType CreateFixture() override { return 5; }
    std::string GetExpectedEncoding() override {
      return OK_STRINGIFY(
        entry {
          key: "v"
          wrapper_typename: "IntKey"
          dyndata {
            vInt: 5
          }
        });
    }
    void CheckValue(const WType &v) override { EXPECT_EQ(5, v); }
  };

  Harness().Run();
}

TEST(SVMapPBTest, TestPBDynSerializationStr) {
  OK_DECLARE_SV_DSTRUL_PBDYNDATA_ENTRY(StrKey, std::string);

  struct Harness : public PBDynSerializationHarness<StrKey> {
    WType CreateFixture() override { return "moof"; }
    std::string GetExpectedEncoding() override {
      return OK_STRINGIFY(
        entry {
          key: "v"
          wrapper_typename: "StrKey"
          dyndata {
            vStr: "moof"
          }
        });
    }
    void CheckValue(const WType &v) override { EXPECT_EQ("moof", v); }
  };

  Harness().Run();
}

TEST(SVMapPBTest, TestPBDynSerializationBytes) {
  OK_DECLARE_SV_DSTRUL_PBDYNDATA_ENTRY(BytesKey, ByteIArray);

  struct Harness : public PBDynSerializationHarness<BytesKey> {
    WType CreateFixture() override {
      static const std::string kFixture = "moof";
      return ByteIArray::FascadeForBuffer(
                            (byte *) kFixture.data(),
                            kFixture.size());
    }
    std::string GetExpectedEncoding() override {
      return OK_STRINGIFY(
        entry {
          key: "v"
          wrapper_typename: "BytesKey"
          dyndata {
            vBytes: "moof"
          }
        });
    }
    void CheckValue(const WType &v) override {
      EXPECT_EQ(4, v.Size());
      EXPECT_EQ('m', v.at(0));
      EXPECT_EQ('o', v.at(1));
      EXPECT_EQ('o', v.at(2));
      EXPECT_EQ('f', v.at(3));
    }
  };

  Harness().Run();
}

TEST(SVMapPBTest, TestPBDynSerializationIntArr) {
  OK_DECLARE_SV_DSTR_PBDYNARR_ENTRY(IntArrKey, IArray<int64_t>);

  struct Harness : public PBDynSerializationHarness<IntArrKey> {
    WType CreateFixture() override {
      static std::vector<int64_t> kFixture = {1, 2};
      return IArray<int64_t>::FascadeForContainer(kFixture);
    }
    std::string GetExpectedEncoding() override {
      return OK_STRINGIFY(
        entry {
          key: "v"
          wrapper_typename: "IntArrKey"
          dyndata {
            vsInt: 1
            vsInt: 2
          }
        });
    }
    void CheckValue(const WType &v) override {
      EXPECT_EQ(2, v.Size());
      EXPECT_EQ(1, v.at(0));
      EXPECT_EQ(2, v.at(1));
    }
  };

  Harness().Run();
}

// = END Test Dyn Serialization Features =========================



TEST(SVStructPBTest, TestPBSerialization) {
  SVStruct s;
  s.SetAttr<TestClassEntry>(TestClass("test"));

  // Add a non-serializable entry
  OK_DECLARE_SV_DSTR_ENTRY(StrKey, std::string);
  s.SetAttr<StrKey>("test2");

  // Sanity check
  EXPECT_EQ(s.GetAttr<TestClassEntry>().val, "test");
  EXPECT_EQ(s.GetAttr<StrKey>(), "test2");

  // Test serialization
  ok_msg::SVMapData pbm;
  EXPECT_TRUE(s.ToPBMessage(pbm));

  // Omit content check, see test above
  auto decoded_pbm =
    PBFactory::LoadFromString<ok_msg::SVMapData>(
      PBFactory::AsTextFormatString(pbm));
  EXPECT_TRUE(decoded_pbm);

  SVStruct decoded_s;
  decoded_s.RegisterEntry<TestClassEntry>();
  EXPECT_TRUE(decoded_s.FromPBMessage(*decoded_pbm));
  EXPECT_TRUE(decoded_s.HasAttr<TestClassEntry>());
  EXPECT_EQ(decoded_s.GetAttr<TestClassEntry>().val, "test");

  // k2 was not encoded
  EXPECT_FALSE(decoded_s.HasAttr<StrKey>());
}



#else // = NO PROTOBUF =======================================================



namespace ok_msg { class SVMapData {}; }

TEST(SVMapPBTest, TestPBSerialization) {

  SVMap m;
  m.SetKey<TestClassEntry>("k1", TestClass("test"));

  ok_msg::SVMapData pbm;
  EXPECT_FALSE(m.ToPBMessage(pbm));
  EXPECT_FALSE(m.FromPBMessage(pbm));
}

TEST(SVStructPBTest, TestPBSerialization) {

  SVStruct s;
  s.SetAttr<TestClassEntry>(TestClass("test"));

  ok_msg::SVMapData pbm;
  EXPECT_FALSE(s.ToPBMessage(pbm));
  EXPECT_FALSE(s.FromPBMessage(pbm));
}

#endif /* OK_ENABLE_PROTOBUF */

using namespace ok;

