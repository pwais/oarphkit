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

#include "ok/fli/Runtime/Session.hpp"
#include "ok/fli/fsl/FSLRegistry.hpp"

#include "ok_test/PBTestUtils.hpp"

using namespace ok;
using namespace ok::fli;

// = Core Tests ==============================================================



TEST(OKFLiSessionTest, TestBasic) {

//  static const std::string kEnv =
//    OK_STRINGIFY(
//      vars {
//        entry {
//          key: "stdin"
//          wrapper_typename: "EnvFunctorEntry"
//          [ok_msg.EnvFunctorEntryDataSVMapEntry_field] {
//            func {
//              alias: "fsl::stdin"
//            }
//          }
//        }
//        entry {
//          key: "snk"
//          wrapper_typename: "EnvFunctorEntry"
//          [ok_msg.EnvFunctorEntryDataSVMapEntry_field] {
//            func {
//              alias: "fsl::Composition"
//              [ok_msg.CompositionInit.composition] {
//                func {
//                  alias: "fsl::oarph"
//                }
//                func {
//                  alias: "fsl::stdout"
//                }
//              }
//            }
//          }
//        }
//        entry {
//          key: "fli.main"
//          wrapper_typename: "EnvFunctorEntry"
//          [ok_msg.EnvFunctorEntryDataSVMapEntry_field] {
//            func {
//              alias: "fsl::Stream"
//              [ok_msg.StreamInit.stream] {
//                src: { name: "stdin" }
//                snk: { name: "snk" }
//              }
//            }
//          }
//        }
//      });

//  Session s;
//
//  s.GetEnvRef().GetRegistryRef().UnionWith(*ok::fli::fsl::CreateFSLRegistry());
//  OKLOG("wee1: \n" << s.GetEnvRef().ToString());
//
//  EXPECT_TRUE(s.ExecFliSpecFromTxt(kEnv));
//  s.GetEnvRef().DecodeAllFunctors();
//  auto f = s.GetEnvRef().GetFunctor("fli.main");
//
//  OKLOG("wee: \n" << s.GetEnvRef().ToString());
//
//  if (f) { f->Call(rt_datumptr()); }


}

