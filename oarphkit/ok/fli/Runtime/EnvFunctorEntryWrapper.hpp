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
#ifndef OK_FLI_RUNTIME_ENVFUNCTORENTRYWRAPPER_HPP_
#define OK_FLI_RUNTIME_ENVFUNCTORENTRYWRAPPER_HPP_

#include <string>

#include "ok/fli/Core/Core.hpp"
#include "ok/fli/Runtime/FuncAttrs.hpp"
#include "ok/SerializationUtils/IPBSerializable.hpp"
#include "ok/SV/SVStruct.hpp"

#if !OK_ENABLE_PROTOBUF
namespace ok_msg { class EnvFunctorEntryData; }
#endif

namespace ok { namespace fli {

struct env; // Forward. Peligro: avoids circular import

// Env entry wrapper =========================================================

/**
 * A Functor wrapper that helps env serialize and manage Functors
 */
class EnvFunctorEntryWrapper :
    public IPBSerializable<ok_msg::EnvFunctorEntryData> {
public:
  // TODO: the interface is env/Registry oriented; make it protected and just
  // make env/registry friends

  EnvFunctorEntryWrapper() : func_(NullTUPtr<FunctorBase>()) { }
  virtual ~EnvFunctorEntryWrapper() { }
  EnvFunctorEntryWrapper(EnvFunctorEntryWrapper&&) = default;
  EnvFunctorEntryWrapper &operator=(EnvFunctorEntryWrapper&&) = default;

  // =========================================================================
  // Interface to the wrapped Functor

  // Have we created the Functor yet?
  inline bool IsCreated() const { return func_.get(); }

  // Return the wrapped Functor or null on error.
  inline FunctorBase::Ptr /* weak */ GetFunctor() const {
    return MakeUnOwned(func_);
  }

  // Try to create and get the wrapped Functor or return null (& log) on error
  inline FunctorBase::Ptr /* weak */ CreateAndGetFunctor(env &env) {
    OKASSERT_CHECK(CreateFunctor(env), "Error trying to create functor");
    return GetFunctor();
  }

  // (Forcibly) create the wrapped Functor and return true only on success
  virtual bool CreateFunctor(env &env);

  // Encode this functor's state (according to any encoding
  // prefs specified in this wrapper's FuncAttrs).  Do nothing
  // (i.e. use existing encoded data) if not IsCreated().
  virtual bool EncodeFunctor(env &env);

  // (Forcibly) set the wrapped Functor to `f`
  void SetFunctor(FunctorBase::Ptr &&f);

  // =========================================================================
  // Utils and Serialization

  inline const SVStruct &GetAttrs() const { return attrs_; }
  inline SVStruct &GetAttrsRef() { return attrs_; }
  inline void SetAttrs(SVStruct &&attrs) { attrs_ = std::move(attrs); }

  virtual std::string ToString() const;

  #if OK_ENABLE_PROTOBUF
  virtual bool FromPBMessage(MessageType &m) override;
  virtual bool ToPBMessage(MessageType &m) override;
  #endif

protected:
  SVStruct attrs_;
  FunctorBase::Ptr /* owned */ func_;

  bool CreateFunctor(env &env, const std::string &alias);
};

// Env Var Entry Declaration =================================================

OK_DECLARE_SV_DSTRUL_IPB_ENTRY(
  EnvFunctorEntry,
  EnvFunctorEntryWrapper,
  ok_msg::EnvFunctorEntryDataSVMapEntry_field);

} /* namespace fli */
} /* namespace ok */

#endif /* OK_FLI_RUNTIME_ENVFUNCTORENTRYWRAPPER_HPP_ */
