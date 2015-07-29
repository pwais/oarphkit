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
#ifndef OK_SERIALIZATIONUTILS_CPUTILS_INL_HPP_
#define OK_SERIALIZATIONUTILS_CPUTILS_INL_HPP_

#ifndef OK_SERIALIZATIONUTILS_CPUTILS_HPP_
#error "Include CPUtils.hpp instead"
#endif

#include <algorithm>
#include <fstream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>

#include <capnp/pretty-print.h>
#include <capnp/serialize.h>
#include <capnp/schema-parser.h>
#include <kj/io.h>
#include <kj/std/iostream.h>

#include "ok/IArray.hpp"
#include "ok/Common.hpp"
#include "ok/Config.hpp"
#include "ok/Demangle.hpp"

namespace ok {

/**
 * hokai. lets:
 *  * Registry working
 *  * Session / compiler working
 *  * get the Fli REPL working
 *  * test the world test with type checking off etc
 *  * clean up canpn stuff; test out what kenton said about nested types
 */

class CPMessageProvider {
public:

  typedef std::unique_ptr<CPMessageProvider> UPtr;

  template <typename ContainerT>
  static UPtr Create(ContainerT &&c) {
    UPtr p(new CPMessageProvider());
    p->m_arr_ = ROByteIArray::WrappingContainer(std::move(c));
    return p;
  }

  template <typename ContainerT>
  static UPtr Create(const ContainerT &s) {
    UPtr p(new CPMessageProvider());
    p->m_arr_ = ROByteIArray::WrappingContainer(ContainerT(s));
    return p;
  }

  static UPtr Create(ROByteIArray &&arr) {
    UPtr p(new CPMessageProvider());
    p->m_arr_ = std::move(arr);
    return p;
  }

  template <typename MT>
  typename MT::Reader GetReader() {
    if (m_arr_.IsEmpty()) { return typename MT::Reader(); }

    const byte *data = m_arr_.Data();
    const size_t size = m_arr_.Size();

    {
      const uint64_t data_size_words = size / sizeof(::capnp::word);
      // Allow us to read the whole array
      ::capnp::ReaderOptions options;
      options.traversalLimitInWords =
          std::max<uint64_t>(options.traversalLimitInWords, data_size_words);

      ::kj::ArrayPtr<const ::capnp::word> segments[1] = {
          ::kj::arrayPtr<const ::capnp::word>(
              (const ::capnp::word *)data,
              data_size_words)
      };
      ::capnp::SegmentArrayMessageReader reader(segments, options);
      return reader.getRoot<MT>();
    }
  }

protected:
  ROByteIArray m_arr_;
};

class InMemoryFileReader : public ::capnp::SchemaFile::FileReader {
public:
  InMemoryFileReader() { }
  virtual ~InMemoryFileReader() { }

  void Register(const std::string &path, ROByteIArray &&schemaFileContent) {
    path_to_data_[path] = std::move(schemaFileContent);
  }

  bool exists(kj::StringPtr path) const {
    return path_to_data_.find(path) != path_to_data_.end();
  }

  kj::Array<const char> read(kj::StringPtr path) const {
    const auto &data = path_to_data_.at(std::string(path));
    return kj::Array<const char>(
        reinterpret_cast<const char *>(data.Data()),
        data.Size(),
        kj::NullArrayDisposer::instance); // TODO: IArray disposer
  }

protected:
  std::unordered_map<std::string, ROByteIArray> path_to_data_;
};

class CPFactory {
public:

//  inline static ::capnp::InputStreamMessageReader ReaderFromPath(
//      const std::string &path) {
//
//    OKLOG_DEBUG("Loading from " << path);
//    return ReaderFromStream(
//        std::ifstream(path.c_str(), std::ios::in | std::ios::binary));
//  }
//
//  // Load a message from `in` and return null on error
//  inline static ::capnp::InputStreamMessageReader ReaderFromStream(
//      std::istream &in,
//      bool large_message_support=true) {
//
//    if (!OKASSERT_CHECK(in.good(), "Input stream in bad state")) {
//      return nullptr;
//    }
//
//    ::capnp::ReaderOptions options;
//    if (large_message_support) {
//      options.traversalLimitInWords = std::numeric_limits<int>::max();
//    }
//    return ::capnp::InputStreamMessageReader(
//              ::kj::std::StdInputStream(in),
//              options);
//  }

//  inline static ::capnp::SegmentArrayMessageReader ReaderFromString(
//      const std::string &pretty_str) {
//
//    OKLOG_DEBUG("Loading from pretty string");
//    return ReaderFromArray(
//              (const byte *)pretty_str.data(),
//              pretty_str.size());
//  }
//
//  template <typename ContainerT>
//  inline static ::capnp::SegmentArrayMessageReader ReaderFromContainer(
//      const ContainerT &c) {
//
//    OKLOG_DEBUG("Loading from a " << DemangledName(typeid(ContainerT)));
//    return ReaderFromArray((const byte *) c.data(), c.size());
//  }
//
//  inline static ::capnp::SegmentArrayMessageReader ReaderFromArray(
//      const byte *data,
//      size_t size) {
//
//    const uint64_t data_size_words = size / sizeof(::capnp::word);
//
//    // Allow us to read the whole array
//    ::capnp::ReaderOptions options;
//    options.traversalLimitInWords =
//        std::max<uint64_t>(options.traversalLimitInWords, data_size_words);
//
//    ::kj::ArrayPtr<const ::capnp::word> segments[1] = {
//        ::kj::arrayPtr(data, data_size_words)
//    };
//
//    return ::capnp::SegmentArrayMessageReader(segments, options);
//  }

  // =========================================================================
  // == WRITING ==============================================================
  // =========================================================================

  template <typename MessageT>
  inline static std::string AsPrettyString(
      const MessageT &m,
      bool newlines=true) {

    if (newlines) {
      return std::string(::kj::StringPtr(::capnp::prettyPrint(m).flatten()));
    } else {
      return std::string(::kj::StringPtr(::kj::str(m)));
    }
  }

};

} /* namespace ok */

#endif /* OK_SERIALIZATIONUTILS_CPUTILS_INL_HPP_ */
