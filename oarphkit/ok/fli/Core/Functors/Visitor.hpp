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

#ifndef OK_FLI_CORE_FUNCTORS_VISITOR_HPP_
#define OK_FLI_CORE_FUNCTORS_VISITOR_HPP_

#include "ok/fli/Core/Core.hpp"

namespace ok { namespace fli {

/**
 * Functor that computes upon and (likely) mutates shared state;
 * a visitor (as in Visitor Pattern) of a stateful object.
 */
template <typename T>
class Visitor : public FunctorBase {
public:
  Visitor() { }
  virtual ~Visitor() { }

  typedef TUPtr<Visitor<T>> Ptr;

  // Override me!
  virtual void Call(T &val) { }

  static Ptr Create() {
    return MakeOwned(new Visitor<T>());
  }

  virtual std::string ToString() const {
    return "Visitor(" + GetSignature() + ")";
  }

protected:

  virtual std::type_index InType() const override { return typeid(T); }
  virtual std::type_index OutType() const override { return typeid(T); }

  virtual rt_datumptr Call(rt_datumptr in) override {
    Call(in.GetRef<T>());
    return in;
      // Return the visited scope such that Visitors can be easily composed
  }

};

} /* namespace fli */
} /* namespace ok */



#endif /* OK_FLI_CORE_FUNCTORS_VISITOR_HPP_ */
