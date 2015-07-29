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

#ifndef OK_FLI_FSL_FSLSHELLCOMMANDS_SETBREAK_HPP_
#define OK_FLI_FSL_FSLSHELLCOMMANDS_SETBREAK_HPP_

#include <iostream>
#include <string>

#include "ok/fli/fsl/FSLShell.hpp"

namespace ok { namespace fli { namespace fsl {

class SetBreakCmd : public FSLShell::ICommand {
public:
  inline std::string GetName() override { return "set_break"; }

  inline virtual std::string GetHelp() override {
    return "Toggle if this shell should break & exit on error";
  }

  inline bool Run(FSLShell &sh) override {
    std::cout << "Set should break on error [true|false|skip]: ";

    std::string option;
    std::cin >> option;
    std::cin.ignore();  // Consume until newline

    std::cout << std::endl;
    if (option == "true") {
      sh.SetShouldBreakOnError(true);
      std::cout << "Shell now breaking on error." << std::endl;
    } else if (option == "false") {
      sh.SetShouldBreakOnError(false);
      std::cout << "Shell now NOT breaking on error." << std::endl;
    } else {
      std::cout << "Shell breaking behavior NOT CHANGED." << std::endl;
    }

    return true;
  }
};

} /* namespace fsl */
} /* namespace fli */
} /* namespace ok */



#endif /* OK_FLI_FSL_FSLSHELLCOMMANDS_SETBREAK_HPP_ */
