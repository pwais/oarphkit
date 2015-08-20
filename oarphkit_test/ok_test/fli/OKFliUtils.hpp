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
#ifndef OK_TEST_FLI_OKFLIUTILS_HPP_
#define OK_TEST_FLI_OKFLIUTILS_HPP_

#include "gtest/gtest.h"

#include "ok/fli/Runtime/env.hpp"
#include "ok/fli/Utils/Serialization/PBEncodingUtils.hpp"
#include "ok/TemplateFu.hpp"

#include "ok_test/OKAssertCheckThrows.hpp"
#include "ok_test/PBTestUtils.hpp"

#if OK_FLI_COMPILETIME_TYPECHECK_ENABLED

  // Compile setting sensitive EQ (e.g. for type_info equality checks)
  #define EXPECT_CT_EQ(a, b) EXPECT_EQ(a, b)

  // EXPECT_CT_THROW
  #if OK_FLI_EXCEPTIONS_ENABLED
    #define EXPECT_CT_THROW(expr) EXPECT_THROW(expr, OK_FLI_TYPECHECK_ERROR_EXCEPTION_TYPE)
    #define EXPECT_CT_THROW_OR_FALSE(expr) \
      EXPECT_THROW(expr, OK_FLI_TYPECHECK_ERROR_EXCEPTION_TYPE)
  #elif OK_OKASSERT_ENABLED
    #define EXPECT_CT_THROW(expr) EXPECT_OKASSERT_THROW(expr)
    #define EXPECT_CT_THROW_OR_FALSE(expr) \
      EXPECT_FALSE(expr)
  #else
    #define EXPECT_CT_THROW(expr) EXPECT_NO_THROW(expr)
    #define EXPECT_CT_THROW_OR_FALSE(expr) EXPECT_FALSE(expr)
  #endif

#else /* no CT checks */

  #define EXPECT_CT_EQ(a, b) do { } while (0)

  #define EXPECT_CT_THROW(expr) EXPECT_NO_THROW(expr)

  #define EXPECT_CT_THROW_OR_FALSE(expr) EXPECT_NO_THROW(expr)

#endif




#if OK_FLI_RUNTIME_TYPECHECK_ENABLED

  // Compile setting sensitive EQ (e.g. for type_info equality checks)
  #define EXPECT_RT_EQ(a, b) EXPECT_EQ(a, b)

  // EXPECT_RT_THROW
  #if OK_FLI_EXCEPTIONS_ENABLED
    #define EXPECT_RT_THROW(expr) EXPECT_THROW(expr, OK_FLI_TYPECHECK_ERROR_EXCEPTION_TYPE)
  #elif OK_OKASSECT_ENABLED
    #define EXPECT_RT_THROW(expr) EXPECT_OKASSECT_THROW(expr)
  #else
    #define EXPECT_RT_THROW(expr) EXPECT_NO_THROW(expr)
  #endif

#else /* no CT checks */

  #define EXPECT_RT_EQ(a, b) do { } while (0)

  #define EXPECT_RT_THROW(expr) EXPECT_NO_THROW(expr)

#endif



namespace ok_test {

/**
 * Given a functor instance `func` and its expected protobuf-text_format
 * encoding `pb_text`, ensure that:
 *  * `pb_text` is a valid pb message
 *  * `func` actually encodes to `pb_text`
 *
 * Returns a Functor decoded from pb_text (or null on error) for the test
 * body to check against `func`.
 */
template <typename FunctorT>
typename FunctorT::Ptr CheckFunctorEncoding(
    FunctorT &f,
    const std::string &pb_func_text,
    bool is_pure=false,
    ok::fli::env *ep=nullptr) {

  using namespace ok;
  using namespace ok::fli;

  env default_env;
  if (!ep) {
    ep = &default_env;
  }

  // Check that `pb_text` is valid
  EXPECT_TRUE(CheckPBTextFormat<ok_msg::Func>(pb_func_text));

  // Check that `pb_text` is a serialization of `func`
  SVStruct attrs;
  #if OK_ENABLE_PROTOBUF
    EXPECT_TRUE(f.ToEncoded(*ep, attrs));
    if (!is_pure) {
      EXPECT_TRUE(attrs.HasAttr<PBFuncAttr>());
      EXPECT_EQ(
        pb_func_text,
        EncodeMessageMatchingFormat(
          attrs.GetAttr<PBFuncAttr>(),
          pb_func_text));
    }
  #else
    // Most Functors will error, but Pure functors may
    // still return success.
    if (is_pure) {
      EXPECT_TRUE(f.ToEncoded(*ep, attrs));
    } else {
      EXPECT_FALSE(f.ToEncoded(*ep, attrs));
    }

    EXPECT_FALSE(attrs.HasAttr<PBFuncAttr>());
  #endif

  // Now decode func from `attrs`
  #if OK_ENABLE_PROTOBUF
    auto f_decoded = ok::fli::MakeOwned(new FunctorT());
    EXPECT_TRUE(f_decoded);
    EXPECT_TRUE(f_decoded->FromEncoded(*ep, attrs));
    return f_decoded;
  #else
    // Can't decode
    return NullTUPtr<FunctorT>();
  #endif
}

template <typename FunctorT>
typename FunctorT::Ptr CheckFunctorEncodingFromFliSpec(
    const std::string &pb_func_text,
    ok::fli::env &e) {

  using namespace ok;
  using namespace ok::fli;
  using namespace ok::fli::tptr;

  // Check that `pb_text` is valid
  EXPECT_TRUE(CheckPBTextFormat<ok_msg::Func>(pb_func_text));

  // Build the Functor from FliSpec
  #if OK_ENABLE_PROTOBUF
    auto m = ok::PBFactory::LoadFromString<ok_msg::Func>(pb_func_text);
    EXPECT_TRUE(m);
    if (!m) { return NullTUPtr<FunctorT>(); }

    auto f = e.CreateFunctor(FuncAttrsFromPB(*m));
    EXPECT_TRUE(f);
    return MakeUnOwned<FunctorT>(f);
  #else
    return NullTUPtr<FunctorT>();
  #endif
}

}

#endif /* OK_TEST_FLI_OKFLIUTILS_HPP_ */
