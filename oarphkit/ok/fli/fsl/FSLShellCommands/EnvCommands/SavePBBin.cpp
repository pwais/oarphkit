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

#include "ok/fli/fsl/FSLShellCommands/EnvCommands/SavePBBin.hpp"

#include <iostream>

#include "ok/fli/Runtime/Session.hpp"

namespace ok { namespace fli { namespace fsl {

bool EnvSavePBBin::Run(FSLShell &sh) {
  auto *e = sh.GetEnv();
  if (!e) {
    std::cout << "No env to save!" << std::endl;
    return false;
  }

  auto s = Session::WrappingEnv(e);

  std::cout << "Save to path: ";
  std::string path;
  std::cin >> path;
  std::cin.ignore();  // Consume until newline

  if (!s.SavePBToPath(path)) {
    std::cout << "Error saving env to " << path << std::endl;
    return false;
  }

  std::cout << "Wrote env to " << path << std::endl;
  return true;
}

} /* namespace fsl */
} /* namespace fli */
} /* namespace ok */
