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

#include "ok/fli/fsl/FSLShell.hpp"

#include "ok/fli/fsl/FSLShellCommands/StandardCommands.hpp"

namespace ok { namespace fli { namespace fsl {

FSLShell::FSLShell()
  : break_on_error_(true),
    env_(nullptr) {

  AddStandardCommands(*this);
}

void FSLShell::Call() {
  looping_ = true;
  while (looping_) {
    std::cout << GetPrompt();
    std::string cmd_name;
    std::cin >> cmd_name;
    std::cin.ignore();  // Consume until newline

    std::cout << std::endl;
    if (cmd_name.empty()) { continue; }

    bool not_found = true;
    for (auto &cmd : GetCommandsRef()) {
      if (cmd_name == cmd->GetName()) {
        not_found = false;
        bool cmd_success = cmd->Run(*this);
        if (!cmd_success && break_on_error_) {
          std::cout << "Command " << cmd_name << " failed." << std::endl;
          looping_ = false;
        }
        break;
      }
    }

    if (not_found) {
      std::cout << "No command: " << cmd_name << "; try help." << std::endl;
    }
  }

  std::cout << "Exiting shell" << std::endl;
}

} /* namespace fsl */
} /* namespace fli */
} /* namespace ok */

