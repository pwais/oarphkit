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

#ifndef OK_FLI_RUNTIME_FUNCATTRS_HPP_
#define OK_FLI_RUNTIME_FUNCATTRS_HPP_

#include <string>

#include "ok/Config.hpp"
#include "ok/SV/SVStruct.hpp"
#include "ok/SerializationUtils/DynamicProto/DynamicProtoUtils.hpp"

#if OK_ENABLE_PROTOBUF
#include "ok_msg/FLiSpec.pb.h"
#include "ok/SerializationUtils/PBUtils.hpp"
#endif

namespace ok { namespace fli {

// = Core Functor Attributes ==================================================

// Should this functor be created lazily, when (if ever) accessed from the env?
OK_DECLARE_SV_DSTRUL_PBDYNDATA_ENTRY(IsLazilyCreatedFuncAttr, bool);

// = PBFuncAttr ================================

#if OK_ENABLE_PROTOBUF

  OK_DECLARE_SV_USTR_ENTRY(
    PBFuncAttr,
    ok_msg::Func,
    ok::PBFactory::AsTextFormatString<ok_msg::Func>);

    // NB: This attribute gets encoded/decoded specially in order to keep
    // FLiSpec text format syntax clean

#else

  OK_DECLARE_SV_DSTR_ENTRY(PBFuncAttr, int);
    // NB: A dummy; w/out protobuf, should never be set

#endif

// = Utils ====================================================================

// Register (for decoding) all func attrs declared here with the given SVStruct
void RegisterFuncAttrs(SVStruct &attrs);

} /* namespace fli */
} /* namespace ok */

#endif /* OK_FLI_RUNTIME_FUNCATTRS_HPP_ */
