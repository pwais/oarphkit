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
#ifndef OK_FLI_FSL_STDINSOURCE_HPP_
#define OK_FLI_FSL_STDINSOURCE_HPP_

#include <iostream>
#include <string>

#include "ok/fli/Core/Functors/Source.hpp"

namespace ok { namespace fli { namespace fsl {

class StdinSource : public Source<std::string> {
public:
  typedef TUPtr<StdinSource> Ptr;
  typedef TUPtr<std::string> OutTypePtr;

  StdinSource() { }
  virtual ~StdinSource() { }

  virtual std::string RegistryAlias() const { return "fsl::stdin"; }

  inline
  virtual OutTypePtr Call() override {
    auto line = MakeOwned(new std::string());
    std::getline(std::cin, *line);
    if (line->empty()) {
      return NullTUPtr<std::string>();
    } else {
      return line;
    }
  }

  static Ptr Create() {
    return MakeOwned(new StdinSource());
  }

  virtual std::string ToString() const {
    return "StdinSource(" + GetSignature() + ")";
  }
};

} /* namespace fsl */
} /* namespace fli */
} /* namespace ok */

#endif /* OK_FLI_FSL_STDINSOURCE_HPP_ */
