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
#ifndef OK_TEST_PBTESTUTILS_HPP_
#define OK_TEST_PBTESTUTILS_HPP_

#include <string>

#include "ok/SerializationUtils/PBUtils.hpp"
#include "ok/TemplateFu.hpp"

namespace ok_test {

template <typename IPBSerializableT>
bool CheckPBSerializableTextFormat(const std::string &text);

template <typename IPBSerializableT>
bool CheckSerializesTo(IPBSerializableT &obj, const std::string &text);

template <typename IPBSerializableT>
bool CheckDecodesFrom(IPBSerializableT &obj, const std::string &text);

template <typename MessageT>
bool CheckPBTextFormat(const std::string &text);

#if OK_ENABLE_PROTOBUF

template <typename MessageT>
std::string EncodeMessageMatchingFormat(
    const MessageT &m,
    const std::string &text) {

  bool newlines = text.find("\n") != std::string::npos;
  std::string encoded = ok::PBFactory::AsTextFormatString(m, newlines);

  // rtrim ending space; artifact from protobuf
  encoded.erase(encoded.find_last_not_of(" \n\r\t") + 1);
  return encoded;
}

template <typename MessageT>
bool CheckPBTextFormat(const std::string &text) {
  auto mptr = ok::PBFactory::LoadFromString<MessageT>(text);
  EXPECT_TRUE(mptr);
  if (!mptr) { return false; }
  auto encoded = EncodeMessageMatchingFormat(*mptr, text);
  EXPECT_EQ(encoded, text);
  return true;
}

template <typename IPBSerializableT>
bool CheckPBSerializableTextFormat(const std::string &text) {
  auto mptr =
      ok::PBFactory::LoadFromString<typename IPBSerializableT::MessageType>(
          text);
  EXPECT_TRUE(mptr);
  if (!mptr) { return false; }

  IPBSerializableT t;
  bool load_success = t.FromPBMessage(*mptr);
  EXPECT_TRUE(load_success);

  auto encoded_mptr = mptr->New(); // Use Protobuf's New() factory
  EXPECT_TRUE(encoded_mptr);
  bool encode_success = t.ToPBMessage(*encoded_mptr);
  EXPECT_TRUE(encode_success);

  bool newlines = text.find("\n") != std::string::npos;
  std::string encoded =
    ok::PBFactory::AsTextFormatString<typename IPBSerializableT::MessageType>(
      *encoded_mptr,
      newlines);

  // rtrim ending space; artifact from protobuf
  encoded.erase(encoded.find_last_not_of(" \n\r\t") + 1);
  EXPECT_EQ(encoded, text);

  delete encoded_mptr;
  return true;
}

template <typename IPBSerializableT>
bool CheckSerializesTo(IPBSerializableT &obj, const std::string &text) {
  // Sanity check: does the text decode?
  // NB: Also allows us to get a message instance...
  auto mptr =
      ok::PBFactory::LoadFromString<typename IPBSerializableT::MessageType>(
          text);
  EXPECT_TRUE(mptr);
  if (!mptr) { return false; }

  auto fresh_mptr = mptr->New(); // Use Protobuf's New() factory

  // Test encoding
  if (!obj.ToPBMessage(*fresh_mptr)) { delete fresh_mptr; return false; }

  // Do we get the same thing?
  bool newlines = text.find("\n") != std::string::npos;
  std::string encoded =
      ok::PBFactory::AsTextFormatString<typename IPBSerializableT::MessageType>(
          *fresh_mptr,
          newlines);

  // rtrim ending space; artifact from protobuf
  encoded.erase(encoded.find_last_not_of(" \n\r\t") + 1);
  EXPECT_EQ(encoded, text);

  delete fresh_mptr;
  return true;
}

template <typename IPBSerializableT>
bool CheckDecodesFrom(IPBSerializableT &obj, const std::string &text) {
  // Decode the message to a PB
  auto mptr =
      ok::PBFactory::LoadFromString<typename IPBSerializableT::MessageType>(
          text);
  EXPECT_TRUE(mptr);
  if (!mptr) { return false; }

  // Decode the PB into the obj
  return obj.FromPBMessage(*mptr);
}

#else

// NB: We can't really test the stubs of the interface since
// the proto class `MessageT` will be incomplete.

template <typename IPBSerializableT>
bool CheckPBSerializableTextFormat(const std::string &text) {
  return true;
}

template <typename IPBSerializableT>
bool CheckSerializesTo(IPBSerializableT &obj, const std::string &text) {
  return true;
}

template <typename IPBSerializableT>
bool CheckDecodesFrom(IPBSerializableT &obj, const std::string &text) {
  return true;
}

template <typename MessageT>
bool CheckPBTextFormat(const std::string &text) {
  return true;
}

#endif

} /* namespace ok_test */



#endif /* OK_TEST_PBTESTUTILS_HPP_ */
