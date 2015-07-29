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

#ifndef OK_FLI_CORE_FUNCTORS_TRANSFORM_HPP_
#define OK_FLI_CORE_FUNCTORS_TRANSFORM_HPP_

#include "ok/fli/Core/Core.hpp"

namespace ok { namespace fli {

// Functor for a function of type T1 -> T2 (where T1 and T2 may be identical)
template <typename InT, typename OutT>
class Transform : public FunctorBase {
public:
  Transform() { }
  virtual ~Transform() { }

  typedef InT XfInType;
  typedef OutT XfOutType;
  typedef TUPtr<Transform<InT, OutT>> Ptr;
  typedef TUPtr<OutT> OutTypePtr;

  virtual OutTypePtr Call(XfInType *arg=nullptr) {
    return NullTUPtr<XfOutType>();
  }

  static Ptr Create() {
    return MakeOwned(new Transform<InT, OutT>());
  }

  virtual std::string ToString() const {
    return "Transform(" + GetSignature() + ")";
  }

  virtual std::string RegistryAlias() const { return "fli::Transform"; }

  virtual std::type_index InType() const override { return typeid(InT); }
  virtual std::type_index OutType() const override { return typeid(OutT); }

protected:

  virtual rt_datumptr Call(rt_datumptr in) override {
    return rt_datumptr::Wrap<OutT>(Call(in.Get<InT>()));
  }

};

} /* namespace fli */
} /* namespace ok */

#endif /* OK_FLI_CORE_FUNCTORS_TRANSFORM_HPP_ */
