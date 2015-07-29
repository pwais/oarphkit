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

#include "ok/fli/fsl/FSLRegistry.hpp"

using namespace ok;
using namespace ok::fli;
using namespace ok::fli::fsl;

TEST(OKFliFSLRegistryTest, TestBasic) {
  auto reg = CreateFSLRegistry();
  EXPECT_EQ("fsl", reg->GetName());
}
