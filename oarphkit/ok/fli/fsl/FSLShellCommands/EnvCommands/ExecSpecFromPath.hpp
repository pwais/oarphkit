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

#ifndef OK_FLI_FSL_FSLSHELLCOMMANDS_ENVCOMMANDS_EXECSPECFROMPATH_HPP_
#define OK_FLI_FSL_FSLSHELLCOMMANDS_ENVCOMMANDS_EXECSPECFROMPATH_HPP_

#include "ok/fli/fsl/FSLShell.hpp"

namespace ok { namespace fli { namespace fsl {

class EnvExecSpecFromPath : public FSLShell::ICommand {
public:
  inline std::string GetName() override { return "env.exec_spec_path"; }

  inline std::string GetHelp() override {
    return "Load and execute FLiSpec from a path";
  }

  bool Run(FSLShell &sh) override;
};

} /* namespace fsl */
} /* namespace fli */
} /* namespace ok */

#endif /* OK_FLI_FSL_FSLSHELLCOMMANDS_ENVCOMMANDS_EXECSPECFROMPATH_HPP_ */
