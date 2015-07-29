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
#ifndef OK_FLI_FSL_STDOUTSINK_HPP_
#define OK_FLI_FSL_STDOUTSINK_HPP_


#include <iostream>
#include <string>

#include "ok/fli/Core/Functors/Sink.hpp"

namespace ok { namespace fli { namespace fsl {

class StdoutSink : public Sink<std::string> {
public:
  typedef TUPtr<StdoutSink> Ptr;
  typedef TUPtr<std::string> OutTypePtr;

  StdoutSink() { }
  virtual ~StdoutSink() { }

  virtual std::string RegistryAlias() const { return "fsl::stdout"; }

  virtual void Call(std::string *arg=nullptr) override {
    OKASSERT_FATAL(arg, "Programming error");
    std::cout << *arg;
  }

  static Ptr Create() {
    return MakeOwned(new StdoutSink());
  }

  virtual std::string ToString() const {
    return "StdoutSink(" + GetSignature() + ")";
  }
};

} /* namespace fsl */
} /* namespace fli */
} /* namespace ok */


#endif /* OK_FLI_FSL_STDOUTSINK_HPP_ */
