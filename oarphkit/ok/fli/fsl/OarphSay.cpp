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

#include "ok/fli/fsl/OarphSay.hpp"

#include <sstream>

namespace ok { namespace fli { namespace fsl {

std::string OarphSay(const std::string &in) {
  std::stringstream ss;
  ss << "                 ";
  for (size_t i = 0; i < in.size(); ++i) { ss << "_"; }
  ss << "_______________\n";


  ss << "                < " << in << (in.empty() ? " " : "!");
  ss <<                       " oarph oparh!>\n";

  ss << "                 ";
  for (size_t i = 0; i < in.size(); ++i) { ss << "-"; }
  ss << "---------------\n";

  //                    __________________
  //                   < $in! oarph oparh!>
  //                    ------------------
  ss <<
      "                   /\n"
      "                  /\n"
      "          -~~~~-\n"
      "         |O __ O|\n"
      "      -__|_\\__/_|__-\n"
      "      ---(__/\\__)---\n"
      "        /  ~--~  \\\n"
      "       /  | \\/ |  \\\n"
      "      |   | /\\ |   |\n"
      "    _/ /\\__    __/\\ \\_\n"
      "  ~~~~~    ~~~~    ~~~~~ \n";

  return ss.str();
}

} /* namespace fsl */
} /* namespace fli */
} /* namespace ok */
