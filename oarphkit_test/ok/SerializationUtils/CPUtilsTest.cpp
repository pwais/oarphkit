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

// Quiet debug output in these tests
#define OK_OKLOG_ENABLE_DEBUG 0

#include "ok/Config.hpp"
#include "ok/SerializationUtils/CPUtils.hpp"

#if OK_ENABLE_CAPNP
// (otherwise, skip all tests)

#include <string>

#include "ok_test_msg/TestMessage.capnp.h"

using namespace ok;
using namespace ok_test_msg::capnpm;

static const std::string kTestMessageAsPrettyString =
    "@0x8123456789abcdef;"
    "using TestMessage = import \"TestMessage.capnp\".TestMessage;"
    "const main :TestMessage = (rawStr = \"oarph\", nums = [8]);";


/**
 * We'll need to use something like what capnp eval does
 *
 *

$ cat tast.capnp
@0xbbea78c6d55644d3;
using Moof = import "/addressbook.capnp";
using AddressBook = Moof.AddressBook;
using Person = Moof.Person;
using Phone = Person.PhoneNumber;

const person :Person = (
    name = "Bob",
    phones = [
      (number = "555-1212", type = mobile),
      import "phone2.capnp".phone
    ]);

const phone :PhoneNumber = (number = "555-1212", type = mobile);

$ ./capnp eval --verbose -I ../c++/samples/ -I ../c++/src/ tast.capnp person
tast.capnp:11:14-28: error: Import failed: phone2.capnp
( id = 0,
  name = "Bob",
  phones = [
    (number = "555-1212", type = mobile),
    (type = mobile) ],
  employment = (unemployed = void) )

also:
 https://groups.google.com/forum/#!searchin/capnproto/text$20format/capnproto/S9t_5yO22NI/djZ6JdKGwkIJ

 *
 */

//TEST(CPFactoryTest, TestReadPrettyString) {
//
//  auto provider = CPMessageProvider::Create(kTestMessageAsPrettyString);
//  auto m = provider->GetReader<TestMessage>();
//  EXPECT_TRUE(m.hasNums());
//  EXPECT_EQ(1, m.getNums().size());
//  EXPECT_EQ(8, m.getNums()[0]);
//  EXPECT_TRUE(m.hasRawStr());
//  EXPECT_EQ("oarph", std::string(m.getRawStr()));
//}

TEST(CPFactoryTest, TestReadPrettyString) {


  InMemoryFileReader fr;
  fr.Register(
      "main.capnp",
      ROByteIArray::FascadeForContainer(kTestMessageAsPrettyString));

  fr.Register(
      "TestMessage.capnp",
      ROByteIArray::WrappingContainer(
        std::string(
          "@0xe9700ced1026bee4;"
          "struct TestMessage {"
          "  rawBytes@0 :Data;"
          "  rawStr@1 :Text;"
          "  nums@2 :List(UInt64);"
          "}")));


  ::capnp::SchemaParser parser;
//  parser.loadCompiledTypeAndDependencies<TestMessage>();

  auto psfile = parser.parseFile(
                  ::capnp::SchemaFile::newDiskFile(
                      "main", "main.capnp", {""}, fr));
  auto psMain = psfile.getNested("main").asConst();

  ::capnp::MallocMessageBuilder builder;
  builder.setRoot(psMain.as<::capnp::DynamicStruct>());
  TestMessage::Reader m = builder.getRoot<TestMessage>();
  EXPECT_EQ("(rawStr = \"oarph\", nums = [8])", ::kj::str(m));

//
//      AddressBook::Reader ab = builder.getRoot<AddressBook>();
//      std::cerr << kj::str(ab).cStr() << std::endl;
//
//
//  capnp::SchemaParser sp;
//      capnp::ParsedSchema ps = sp.parseDiskFile(
//                                    "myObj"
//                                  , "Test.cfg"
//                                  , {""}
//                          );
//      capnp::ParsedSchema ps2 = ps.getNested("myObj");
//      capnp::ConstSchema cs = ps2.asConst();
//
//      capnp::MallocMessageBuilder builder;
//      builder.setRoot(cs.getProto());
//  builder.setRoot(cs.as<capnp::DynamicStruct>());

}

TEST(CPFactoryTest, TestWritePrettyString) {
  ::capnp::MallocMessageBuilder mb;
  TestMessage::Builder b = mb.initRoot<TestMessage>();
  auto nums = b.initNums(1);
  nums.set(0, 8);
  b.setRawStr("oarph");
  ::capnp::SegmentArrayMessageReader reader(mb.getSegmentsForOutput());
  auto s = CPFactory::AsPrettyString(
      reader.getRoot<::capnp::DynamicStruct>(::capnp::Schema::from<TestMessage>()),
      false);
  EXPECT_EQ("(rawStr = \"oarph\", nums = [8])", s);
}

//TEST(PBFactoryTest, TestToFromArray) {
//  TestMessage m;
//  m.set_raw_str("oarph");
//  m.add_nums(8);
//
//  auto arr = PBFactory::AsArray(m);
//  EXPECT_FALSE(arr.IsEmpty());
//  EXPECT_EQ(m.ByteSize(), arr.Size());
//
//  TestMessageUPtr dm = PBFactory::LoadFromContainer<TestMessage>(arr);
//  EXPECT_TRUE(dm);
//  EXPECT_TRUE(dm->nums_size() > 0);
//  EXPECT_EQ(8, dm->nums(0));
//  EXPECT_TRUE(dm->has_raw_str());
//  EXPECT_EQ("oarph", dm->raw_str());
//}

#endif
