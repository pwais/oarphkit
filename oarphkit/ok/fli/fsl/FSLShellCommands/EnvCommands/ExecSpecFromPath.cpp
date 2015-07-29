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

#include "ok/fli/fsl/FSLShellCommands/EnvCommands/ExecSpecFromPath.hpp"

#include <iostream>

#include "ok/fli/Runtime/Session.hpp"

namespace ok { namespace fli { namespace fsl {

bool EnvExecSpecFromPath::Run(FSLShell &sh) {
  auto *e = sh.GetEnv();
  Session s;
  if (e) {
    s.WrapEnv(MakeUnOwned(e));
  } else {
    std::cout << "Warning: running on a temporary env" << std::endl;
    s.InitEmptyEnv();
  }

  std::cout << "Load from path: ";
  std::string path;
  std::cin >> path;
  std::cin.ignore();  // Consume until newline

  return s.ExecFliSpec(path);
}

} /* namespace fsl */
} /* namespace fli */
} /* namespace ok */

