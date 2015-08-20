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

#ifndef OK_FLI_UTILS_SERIALIZATION_PBENCODINGUTILS_HPP_
#define OK_FLI_UTILS_SERIALIZATION_PBENCODINGUTILS_HPP_

#include "ok/Config.hpp"

#include "ok/fli/Runtime/env.hpp"
#include "ok/fli/Runtime/FuncAttrs.hpp"

#if OK_ENABLE_PROTOBUF
#include "ok_msg/FLiSpec.pb.h"
#else
namespace ok_msg { class Func; }
#endif

/**
 * This module contains some utilities for functors that support
 * PBFuncAttr encoding.  The utilities primarily contain error-
 * checking boilerplate and are intended for use within
 * Functor {To,From}Encoded member functions.
 */

namespace ok { namespace fli {

#if OK_ENABLE_PROTOBUF

  // Encode a functor and return false upon error
  template <typename BlockT>
  inline
  bool EncodeToPBFunc(
      env &env,
      SVStruct &m,
      const std::string &alias,
      BlockT block) {

    if (!env.HasEncoding<PBFuncAttr>()) {
      return true; // No errors
    }

    ok_msg::Func spec;
    spec.set_alias(alias);
    bool no_errors = block(spec);
    if (!OKASSERT_CHECK(no_errors, "Encoding error")) { return false; }

    m.SetAttr<PBFuncAttr>(ok_msg::Func());
    m.GetAttrRef<PBFuncAttr>().Swap(&spec);
    return true;
  }

  // Encode a functor to dyndata and return false upon error
  template <typename BlockT>
  inline
  bool EncodeToPBFuncDyn(
      env &env,
      SVStruct &m,
      const std::string &alias,
      BlockT block_dyn) {

    if (!env.HasEncoding<PBFuncAttr>()) {
      return true; // No errors
    }

    ok_msg::Func spec;
    spec.set_alias(alias);
    bool no_errors = block_dyn(*spec.mutable_fli_dyn_params());
    if (!OKASSERT_CHECK(no_errors, "Encoding error")) { return false; }

    m.SetAttr<PBFuncAttr>(ok_msg::Func());
    m.GetAttrRef<PBFuncAttr>().Swap(&spec);
    return true;
  }

  // Decode a functor and return false upon error
  template <typename ExtensionT, typename BlockT>
  inline
  bool DecodeFromPBFunc(
      env &env,
      SVStruct &m,
      const ExtensionT &ext,
      BlockT block) {

    if (!env.HasEncoding<PBFuncAttr>()) {
      return true; // No errors
    }
    OKASSERT_FATAL(m.HasAttr<PBFuncAttr>(), "Missing func attrs");
      // Programming error; user should disable PB on env, else we expect it

    ok_msg::Func &spec = m.GetAttrRef<PBFuncAttr>();
    if (!OKASSERT_CHECK(
           spec.HasExtension(ext),
           "Missing extension: " + PBFactory::AsTextFormatString(spec))) {
      return false;
    }

    bool no_errors = block(spec);
    if (!OKASSERT_CHECK(no_errors, "Decoding error")) { return false; }

    return true;
  }

  // Decode a functor from dyndata and return false upon error
  template <typename BlockT>
  inline
  bool DecodeFromPBFuncDyn(
      env &env,
      SVStruct &m,
      BlockT block_dyn) {

    if (!env.HasEncoding<PBFuncAttr>()) {
      return true; // No errors
    }
    OKASSERT_FATAL(m.HasAttr<PBFuncAttr>(), "Missing func attrs");
      // Programming error; user should disable PB on env, else we expect it

    ok_msg::Func &spec = m.GetAttrRef<PBFuncAttr>();
    bool no_errors = block_dyn(*spec.mutable_fli_dyn_params());
    if (!OKASSERT_CHECK(no_errors, "Decoding error")) { return false; }

    return true;
  }

  // Move the PB-encoded message `spec` into a func attributes SVStruct
  inline
  SVStruct FuncAttrsFromPB(ok_msg::Func &spec) {
    SVStruct f_attrs;
    f_attrs.SetAttr<PBFuncAttr>(ok_msg::Func());
    f_attrs.GetAttrRef<PBFuncAttr>().Swap(&spec);
    return f_attrs; // moves
  }

  // Write into `m` a stateless functor record for `f` and return true
  // once a (correct) encoded message exists
  inline
  bool EncodeStatelessFunctor(env &env, SVStruct &m, FunctorBase *f) {
    if (!env.HasEncoding<PBFuncAttr>()) {
      return false; // Encoded nothing
    }

    // Skip if Functor already encoded a PB
    if (m.HasAttr<PBFuncAttr>()) { return true; }

    if (!OKASSERT_CHECK(f, "Programming error: null Functor")) {
      return false; // Error
    }

    m.SetAttr<PBFuncAttr>(ok_msg::Func());
    m.GetAttrRef<PBFuncAttr>().set_alias(f->RegistryAlias());
    return true;
  }

#else /* no protobuf */

  template <typename BlockT>
  bool EncodeToPBFunc(
      env &env,
      SVStruct &m,
      const std::string &alias,
      BlockT block) {

    if (env.HasEncoding<PBFuncAttr>()) {
      OKLOG_DEBUG("Protobuf not available");
    }
    return false; // Encoded nothing
  }

  template <typename ExtensionT, typename BlockT>
  bool DecodeFromPBFunc(env &env, SVStruct &m, BlockT block) {
    if (m.HasAttr<PBFuncAttr>()) {
      OKLOG_ERROR("Protobuf not avialable but needed for decoding");
    }
    return false; // Decoded nothing
  }

  inline
  bool EncodeStatelessFunctor(env &env, SVStruct &m, FunctorBase *f) {
      return false; // Encoded nothing
  }

#endif


} /* namespace fli */
} /* namespace ok */

#endif /* OK_FLI_UTILS_SERIALIZATION_PBENCODINGUTILS_HPP_ */
