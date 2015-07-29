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
#ifndef OK_FLI_ENV_HPP_
#define OK_FLI_ENV_HPP_

#include <atomic>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "ok/fli/Core/Core.hpp"
#include "ok/fli/Runtime/Registry.hpp"
#include "ok/fli/Utils/EnvAnonCounter.hpp"
#include "ok/SV/SVMap.hpp"
#include "ok/SerializationUtils/IPBSerializable.hpp"

namespace ok_msg { class EnvState; } // Forward

namespace ok { namespace fli {

// The env's Functor Registry
OK_DECLARE_SV_DSTRUL_ENTRY(EnvRegistry, Registry);

// A counter to help Env generate distinct variable names
OK_DECLARE_SV_DSTRUL_IPB_OF_DYNDATA_ENTRY(EnvAnonCounterAttr, EnvAnonCounter);

// When encoding Functors, use these encoding(s) (or all encodings if
// unset / empty)
OK_DECLARE_SV_DSTRUL_ENTRY(
  EncodingPrefsFuncAttr,
  std::unordered_set<std::string>);

// The canonical Functor to run (usually a Subroutine)
OK_DECLARE_SV_DSTRUL_PBDYNDATA_ENTRY(EnvMainAttr, std::string);

// An environment for FLi and Functors to read (and sometimes write ... )
// 'global' data
struct env final : IPBSerializable<ok_msg::EnvState> {

  // == Env Data =============================================================

  // env is essentially a sole map of varname -> object
  SVMap vars;

  // == Functor Interop ======================================================

  // == Creating Functors ======================

  // Creates and returns a Functor with attributes `func_attrs` and
  // returns null on error.  If `func_attrs` indicates a reference to
  // an existing Functor, simply returns that Functor without
  // creating anything.  Postcondition: owns / disposes of `func_attrs`
  FunctorBase::Ptr CreateFunctor(SVStruct &&func_attrs);

  // Assigns the given functor `f` to the variable name `varname`
  void AssignFunctor(const std::string &varname, FunctorBase::Ptr &&f);

  // == Inspecting Functors ====================

  // Get (and possibly lazy-create) the functor bound to the given
  // `varname` (and return null on error)
  FunctorBase::Ptr /* weak */ GetFunctor(const std::string &varname);

  // Creates a map of Functor * to variable name for all known Functors
  std::unordered_map<FunctorBase *, std::string> GetFunctorToName() const;

  // Is `varname` a functor, or something else?
  bool IsFunctor(const std::string &varname) const;

  // == Serializing Functors ===================

  /*
   * Env does not (de)serialize Functors by default.  Use this interface
   * to force Functors to (de)serialize themselves.
   */

  // Tell all Functors to save state to an encoded form. NB: functors
  // may *move* state into an encoded buffer and become invalid.
  bool EncodeAllFunctors();

  // Forcibly create and decode all Functors. (Call after FromPBMessage())
  bool DecodeAllFunctors(bool include_lazy=false);

  /*
   * By default, Functors will use all encodings available.  Tell Env
   * to use a specific encoding using the interface below before
   * calling EncodeAllFunctors().
   */

  // Tell all Functors to encode as `EntryT` (e.g. PBFuncAttr)
  template <typename EntryT>
  void AddFunctorEncodingPref();

  // Allow Functors to ask env if `EntryT` is a requested encoding
  template <typename EntryT>
  bool HasEncoding() const;

  // == Main ===================================

  // Run the env.main functor, if it exists, and return true on success
  bool RunMain();

  // Set the env.main functor to the given Functor and return true on success
  bool SetMain(FunctorBase::Ptr /* weak */ f);
  bool SetMain(const std::string &varname);

  void ClearMain();

  // Get the env.main functor, if it exists, of nullptr on error
  FunctorBase::Ptr /* weak */ GetMain();


  // == Multi-Env ============================================================

  // Create and return a copy of this env that contains the same core
  // (e.g. Registry, encoding prefs, etc) but none of the other vars
  env EmptyClone() const;

  // Move `varname` to the given `other` env; `other` will now own `varname`
  void MoveVarTo(const std::string &varname, env &e);

  // Move all `vars` to the given `other` env; `other` will now own those vars
  template <typename ContainerStrT>
  void MoveVarsTo(const ContainerStrT &vars, env &e);

  // Create and return a new env (based on this one) and move `vars` to
  // that env
  template <typename ContainerStrT>
  inline
  env Splice(const ContainerStrT &vars);

  // Absorb all vars, Registry settings, encodings, etc., from `other`
  // into this env.  (Postcondition: disposes of `env`).
  void Union(env &&other);



  // == Utils ================================================================

  inline
  Registry &GetRegistryRef() {
    return vars.GetValueRef<EnvRegistry>("env.root_registry");
  }

  inline
  const Registry &GetRegistry() const {
    return vars.GetValue<EnvRegistry>("env.root_registry");
  }

  /**
   * (Atomically) creates and returns a distinct variable name
   * for an 'anonymous' variable (optionally with the given `prefix`).
   * Thread-safe; never returns the same name twice.  Checks to avoid
   * choosing an existing variable name.
   *
   * NB: does not *reserve* the variable name.  To preclude naming
   * collisions, use a `prefix` to namespace your variables.
   *
   * If a distinct variable name could not be found (e.g. the env is FULL),
   * returns (non-distinct) `prefix` and logs an error.
   */
  std::string CreateDistinctAnonName(const std::string &prefix="");

  // Returns true if `varname` is reserved for the env namespace
  static bool IsEnvVar(const std::string &varname);

  std::string ToString() const;

  #if OK_ENABLE_PROTOBUF
  bool FromPBMessage(MessageType &m) override;
  bool ToPBMessage(MessageType &m) override;
  #endif

  typedef std::unique_ptr<env> uptr;
  typedef TUPtr<env> tptr;

  env(); // Set up some core variables
  virtual ~env() { }

  // env is only movable
  env(const env &) = delete;
  env &operator=(const env &) = delete;
  env(env &&) = default;
  env &operator=(env &&) = default;
};

} /* namespace fli */
} /* namespace ok */

#include "ok/fli/Runtime/env-inl.hpp"

#endif /* OK_FLI_ENV_HPP_ */
