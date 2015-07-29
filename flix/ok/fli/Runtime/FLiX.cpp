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

#include <string>

#include "ok/fli/Runtime/Session.hpp"

int main(int argc, char* argv[]) {
  using namespace ok::fli;
 
  OKLOG_DEBUG("Starting FLiX...");

  auto s = Session::CreateStandardSession();
  bool success = true;
  for (size_t arg = 1; arg < argc; ++arg) {
    success &= s->ExecFliSpec(std::string(argv[arg]));
  }

  OKLOG_DEBUG("...FLiX complete with success: " << success);

  return !success;
}

