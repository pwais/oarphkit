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

#ifndef OK_FLI_FSL_OARPHSAY_HPP_
#define OK_FLI_FSL_OARPHSAY_HPP_

#include <string>

#include "ok/fli/Core/Functors/FuncWrapper.hpp"

namespace ok { namespace fli { namespace fsl {

std::string OarphSay(const std::string &in);

OK_FLI_DECLARE_FW(OarphSayXform, OarphSay, "fsl::oarph");

} /* namespace fsl */
} /* namespace fli */
} /* namespace ok */

#endif /* OK_FLI_FSL_OARPHSAY_HPP_ */
