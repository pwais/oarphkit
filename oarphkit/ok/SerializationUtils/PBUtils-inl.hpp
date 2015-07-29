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
#ifndef OK_SERIALIZATIONUTILS_PBUTILS_INL_HPP_
#define OK_SERIALIZATIONUTILS_PBUTILS_INL_HPP_

#ifndef OK_SERIALIZATIONUTILS_PBUTILS_HPP_
#error "Include PBUtils.hpp instead"
#endif

#if !OK_ENABLE_PROTOBUF
#error "This module requires protobuf"
#endif

#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/coded_stream.h>

#include "ok/Common.hpp"
#include "ok/Config.hpp"
#include "ok/Demangle.hpp"
#include "ok/IArray.hpp"

namespace ok {

template <typename MT>
using MUPtr = std::unique_ptr<MT>;

class PBFactory {
public:

  // =========================================================================
  // == READING ==============================================================
  // =========================================================================

  template <typename MT>
  static MUPtr<MT> LoadFromPath(const std::string &path) {
    OKLOG_DEBUG("Loading a " << MessageTypeName<MT>() << " from " << path);
    std::ifstream inf(path, std::ios::in | std::ios::binary);
    return LoadFromStream<MT>(inf);
  }

  // Load a message from `in` and return null on error
  template <typename MT>
  static MUPtr<MT> LoadFromStream(
      std::istream &in,
      bool large_message_support=true) {

    if (!OKASSERT_CHECK(in.good(), "Input stream in bad state")) {
      return nullptr;
    }

    VerifyProfobuf();

    ///
    /// First, try reading Binary
    ///
    OKLOG_DEBUG("Trying binary ...");
    if (large_message_support) {
      google::protobuf::io::IstreamInputStream pb_iis(&in);
      MUPtr<MT> message = LoadLargeFromPbInputStream<MT>(pb_iis);
      if (message) { return message; }
    } else {
      // Use protobuf's built-in limits
      MUPtr<MT> message(new MT());
      if (message->ParseFromIstream(&in)) { return message; }
    }

    ///
    /// Didn't read & return? Try TextFormat
    ///
    OKLOG_DEBUG("... trying text_format ...");

    in.clear();
    in.seekg(0, std::ios::beg);
    if (!OKASSERT_CHECK(in.good(), "Failed to rewind stream")) {
      return nullptr;
    }

    std::stringstream ss;
    ss << in.rdbuf();
    MUPtr<MT> message(new MT());
    const bool success =
      ::google::protobuf::TextFormat::ParseFromString(ss.str(), message.get());
    if (success) { return message; }

    OKLOG_DEBUG("... failed to read a " << MessageTypeName<MT>());
    return nullptr;
  }

  template <typename MT>
  static MUPtr<MT> LoadFromString(const std::string &text_format_str) {
    OKLOG_DEBUG(
      "Loading a " << MessageTypeName<MT>() << " from text_format string");
    return LoadTextFormatFromArray<MT>(
              (const byte *)text_format_str.data(),
              text_format_str.size());
  }

  template <typename MT, typename ContainerT>
  static MUPtr<MT> LoadFromContainer(const ContainerT &c) {
    OKLOG_DEBUG(
      "Loading a " << MessageTypeName<MT>() <<
      " from a " <<
      DemangledName(typeid(ContainerT)));
    return LoadFromArray<MT>((const byte *) c.data(), c.size());
  }

  template <typename MT>
  static MUPtr<MT> LoadFromArray(const byte *data, size_t size) {
    if (!OKASSERT_CHECK((data != nullptr) && (size > 0), "Bad array")) {
      return nullptr;
    }

    VerifyProfobuf();

    ///
    /// First, try reading Binary
    ///
    OKLOG_DEBUG("Trying binary ...");
    google::protobuf::io::ArrayInputStream pb_ais(data, size);
    MUPtr<MT> message = LoadLargeFromPbInputStream<MT>(pb_ais);
    if (message) { return message; }

    ///
    /// Didn't read & return? Try TextFormat
    ///
    OKLOG_DEBUG("... trying text_format ...");
    message = LoadTextFormatFromArray<MT>(data, size);
    if (message) { return message; }

    OKLOG_DEBUG("... failed to read a " << MessageTypeName<MT>());
    return nullptr;
  }

  // =========================================================================
  // == WRITING ==============================================================
  // =========================================================================

  template <typename MT>
  static std::string AsTextFormatString(
      const MT &m,
      bool newlines=true) {

    VerifyProfobuf();

    ::google::protobuf::TextFormat::Printer tfp;
    tfp.SetSingleLineMode(!newlines);

    std::string out;
    if (OKASSERT_CHECK(
          tfp.PrintToString(m, &out),
          "Error writing a " + MessageTypeName<MT>() + " in TextFormat")) {
      return out;
    } else {
      return "";
    }
  }

  template <typename MT>
  static ByteIArray AsArray(const MT &m) {
    VerifyProfobuf();
    auto arr = ByteIArray::Create(m.ByteSize());
    if (OKASSERT_CHECK(
          m.SerializeToArray(arr.Data(), arr.Size()),
          "Error writing a " + MessageTypeName<MT>() + " to array")) {
      return arr;
    } else {
      return ByteIArray();
    }
  }

  template <typename MT>
  static bool SaveBinToPath(const MT &m, const std::string &path) {
    VerifyProfobuf();
    std::ofstream out(path, std::ios::out | std::ios::binary);
    if (!OKASSERT_CHECK(out.good(), "Bad destination: " + path)) {
      return false;
    }

    bool write_success =
      OKASSERT_CHECK(
        m.SerializeToOstream(&out),
        "Error writing a " + MessageTypeName<MT>() + " to " + path);

    out.close();
    return write_success;
  }

protected:

  // =========================================================================
  // == UTILS ================================================================
  // =========================================================================

  static void VerifyProfobuf() {
    OKLOG_DEBUG(
      "Using protobuf version " << GOOGLE_PROTOBUF_VERSION);

    // TODO: nice way to disable version check for release
    #if !OK_OKASSERT_DISABLE_FATAL
      GOOGLE_PROTOBUF_VERIFY_VERSION;
    #endif
  }

  template <typename MT>
  static std::string MessageTypeName() {
    return DemangledName(typeid(MT));
  }

  template <typename MT, typename PBInputStreamT>
  static MUPtr<MT> LoadLargeFromPbInputStream(PBInputStreamT &pb_iis) {
    try {
      /**
       * Support reading arbitrarily large messages. This feature is a
       * "security hazard" since an attacker could DOS/OOM the machine,
       * but in practice often necessary because protobuf only supports
       * 64MB messages by default.
       */
      google::protobuf::io::CodedInputStream cis(&pb_iis);
      cis.SetTotalBytesLimit(
        std::numeric_limits<int>::max(),        // Support very large messages
        0.5 * std::numeric_limits<int>::max()); // Warn when halfway there

      MUPtr<MT> message(new MT());
      if (message->ParseFromCodedStream(&cis)) { return message; }
    } catch (std::exception &ex) {
      // These can be false positives b/c e.g. the message is in TextFormat
      OKLOG_DEBUG(
        "Exception while trying to read protobuf message: " <<
        ex.what() << "\n" <<
        "(Skipping read-from-CodedInputStream)");
    }

    return nullptr;
  }

  template <typename MT>
  static MUPtr<MT> LoadTextFormatFromArray(const byte *data, size_t size) {
    MUPtr<MT> message = MUPtr<MT>(new MT());
    google::protobuf::io::ArrayInputStream pb_ais(data, size);
    if (::google::protobuf::TextFormat::Parse(&pb_ais, message.get())) {
      return message;
    } else {
      return nullptr;
    }
  }

  // TODO: size-delimited / streaming record stuff.  Kenton argues
  // one doesn't need the size delimiter tho...
  // FMI https://github.com/sandstorm-io/capnproto/blob/4bbc78b87b05a1ecf7e71dfc3869c769c8110d49/c%2B%2B/src/benchmark/protobuf-common.h#L73
};

} /* namespace ok */

#endif /* OK_SERIALIZATIONUTILS_PBUTILS_INL_HPP_ */
