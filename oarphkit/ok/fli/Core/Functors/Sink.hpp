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

#ifndef OK_FLI_CORE_FUNCTORS_SINK_HPP_
#define OK_FLI_CORE_FUNCTORS_SINK_HPP_

#include "ok/fli/Core/Core.hpp"

namespace ok { namespace fli {

// Functor for a function of type T -> void
template <typename T>
class Sink : public FunctorBase {
public:
  Sink() { }
  virtual ~Sink() { }

  typedef T ValType;
  typedef TUPtr<Sink<T>> Ptr;

  // Override me!
  virtual void Call(ValType *arg=nullptr) { }

  static Ptr Create() {
    return MakeOwned(new Sink<ValType>());
  }

  virtual std::string ToString() const {
    return "Sink(" + GetSignature() + ")";
  }

protected:

  virtual std::type_index InType() const override { return typeid(ValType); }
  virtual std::type_index OutType() const override { return typeid(Void); }

  virtual rt_datumptr Call(rt_datumptr in) override {
    Call(in.Get<T>());
    return rt_datumptr();
  }

};

} /* namespace fli */
} /* namespace ok */

#endif /* OK_FLI_CORE_FUNCTORS_SINK_HPP_ */
