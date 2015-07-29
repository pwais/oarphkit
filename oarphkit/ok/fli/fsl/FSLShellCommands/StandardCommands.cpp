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

#include "ok/fli/fsl/FSLShellCommands/StandardCommands.hpp"

#include "ok/fli/fsl/FSLShell.hpp"
#include "ok/fli/fsl/FSLShellCommands/EnvCommands/ExecSpecFromPath.hpp"
#include "ok/fli/fsl/FSLShellCommands/EnvCommands/ExecSpecRaw.hpp"
#include "ok/fli/fsl/FSLShellCommands/EnvCommands/Print.hpp"
#include "ok/fli/fsl/FSLShellCommands/EnvCommands/RunMain.hpp"
#include "ok/fli/fsl/FSLShellCommands/EnvCommands/SavePBBin.hpp"
#include "ok/fli/fsl/FSLShellCommands/EnvCommands/SaveTxt.hpp"
#include "ok/fli/fsl/FSLShellCommands/Exit.hpp"
#include "ok/fli/fsl/FSLShellCommands/Help.hpp"
#include "ok/fli/fsl/FSLShellCommands/SetBreak.hpp"

namespace ok { namespace fli { namespace fsl {

void AddStandardCommands(FSLShell &sh) {
  sh.AddCommand<HelpCmd>();
  sh.AddCommand<ExitCmd>();
  sh.AddCommand<SetBreakCmd>();

  sh.AddCommand<EnvPrintCmd>();
  sh.AddCommand<EnvRunMain>();
  sh.AddCommand<EnvExecSpecFromPath>();
  sh.AddCommand<EnvExecSpecRaw>();
  sh.AddCommand<EnvSavePBBin>();
  sh.AddCommand<EnvSaveTxt>();
}

} /* namespace fsl */
} /* namespace fli */
} /* namespace ok */
