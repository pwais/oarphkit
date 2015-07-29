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

#ifndef OK_FLI_FSL_FSLSHELLCOMMANDS_ENVCOMMANDS_PRINT_HPP_
#define OK_FLI_FSL_FSLSHELLCOMMANDS_ENVCOMMANDS_PRINT_HPP_

#include <iostream>

#include "ok/fli/fsl/FSLShell.hpp"
#include "ok/fli/Runtime/env.hpp"

namespace ok { namespace fli { namespace fsl {

class EnvPrintCmd : public FSLShell::ICommand {
public:
  inline std::string GetName() override { return "env.print"; }

  inline std::string GetHelp() override { return "Print the env"; }

  inline bool Run(FSLShell &sh) override {
    auto *e = sh.GetEnv();
    if (e) {
      std::cout << std::endl << e->ToString() << std::endl;
    } else {
      std::cout << "(No env!)" << std::endl;
    }
    return true;
  }
};

} /* namespace fsl */
} /* namespace fli */
} /* namespace ok */

#endif /* OK_FLI_FSL_FSLSHELLCOMMANDS_ENVCOMMANDS_PRINT_HPP_ */
