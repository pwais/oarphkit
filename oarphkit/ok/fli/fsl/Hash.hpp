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
#ifndef OK_FLI_FSL_HASH_HPP_
#define OK_FLI_FSL_HASH_HPP_

#include <functional>
#include <string>

#include "ok/fli/Core/Functors/Transform.hpp"
#include "ok/Stringify.hpp"

namespace ok { namespace fli { namespace fsl {

// TODO: deleteme?
class Hash : public Transform<std::string, std::string> {
public:
  typedef TUPtr<Hash> Ptr;
  typedef TUPtr<std::string> OutTypePtr;

  virtual std::string RegistryAlias() const { return "fsl::hash"; }

  Hash() { }
  virtual ~Hash() { }

  virtual OutTypePtr Call(std::string *arg=nullptr) {
    OKASSERT_FATAL(arg, "Programming error");
    size_t hashed = hash_(*arg);
    return MakeOwned(new std::string(Stringify(hashed)));
  }

  static Ptr Create() {
    return MakeOwned(new Hash());
  }

  virtual std::string ToString() const {
    return "Hash(" + GetSignature() + ")";
  }

protected:
  std::hash<std::string> hash_;
};

} /* namespace fsl */
} /* namespace fli */
} /* namespace ok */

#endif /* OK_FLI_FSL_HASH_HPP_ */
