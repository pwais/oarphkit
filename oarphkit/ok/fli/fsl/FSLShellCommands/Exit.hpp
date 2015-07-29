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

#ifndef OK_FLI_FSL_FSLSHELLCOMMANDS_EXIT_HPP_
#define OK_FLI_FSL_FSLSHELLCOMMANDS_EXIT_HPP_

#include "ok/fli/fsl/FSLShell.hpp"

namespace ok { namespace fli { namespace fsl {

class ExitCmd : public FSLShell::ICommand {
public:
  inline std::string GetName() override { return "exit"; }

  inline virtual std::string GetHelp() override { return "Exit this shell"; }

  inline bool Run(FSLShell &sh) override {
    sh.SetShouldContinueLooping(false);
    return true;
  }
};

} /* namespace fsl */
} /* namespace fli */
} /* namespace ok */

#endif /* OK_FLI_FSL_FSLSHELLCOMMANDS_EXIT_HPP_ */
