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
#ifndef OK_FLI_REGISTRY_HPP_
#define OK_FLI_REGISTRY_HPP_

#include <list>
#include <unordered_map>
#include <string>

#include "ok/fli/Core/Core.hpp"

namespace ok { namespace fli {

struct env; // Forward

namespace detail {
template <typename FunctorT=FunctorBase>
FunctorBase::Ptr CreateFunctor() {
  return MakeOwned<FunctorT>(new FunctorT());
}
} /* namespace detail */

// A factory for functors
class Registry final {
public:
  typedef std::unique_ptr<Registry> UPtr;

  // == Utils ================================================================

  Registry() : name_("default_registry") { }
  ~Registry() { }

  // Prefer moves
  Registry(Registry&&) = default;
  Registry &operator=(Registry&&) = default;

  inline std::string GetName() const noexcept { return name_; }
  inline void SetName(const std::string &name) { name_ = name; }
  std::string ToString() const;

  // Return all registered aliases (sorted alphabetically)
  std::string GetAllAliases(const std::string &delimiter="\n") const;

  // == Functor Factory Management ===========================================

  // Register a Functor with the given `alias` so that this Registry
  // can create instances of `FunctorT`
  template <typename FunctorT>
  void Register() {
    alias_to_factory_[FunctorT().RegistryAlias()] =
        &detail::CreateFunctor<FunctorT>;
  }

  // Add all `other`'s function factory registrations to `this`
  bool UnionWith(const Registry &other);

  template <typename RegistrySeqT>
  bool UnionWithAll(const RegistrySeqT &regs) {
    bool all_accepted = true;
    for (const auto &reg : regs) {
      all_accepted &= UnionWith(reg);
    }
    return all_accepted;
  }

  // == Functor Creation =====================================================

  inline
  bool IsRegistered(const std::string &alias) const {
    return alias_to_factory_.find(alias) != alias_to_factory_.end();
  }

  // Create and return a default-constructed Functor with `alias`
  // and null on error
  inline
  FunctorBase::Ptr CreateFunctor(const std::string &alias) const {
    if (!OKASSERT_CHECK(
          IsRegistered(alias),
          "Don't know how to create " + alias)) {
      return NullTUPtr<FunctorBase>();
    }

    const auto &f_construct = alias_to_factory_.at(alias);
    return f_construct();
  }

  /**
   * Creates and returns a functor with attributes `func_attrs` and
   * returns null on error.
   *
   * The pointer returned is weak if the `func_attrs` references
   * an env variable name; otherwise, returns a strong pointer.
   */
  FunctorBase::Ptr CreateFunctor(SVStruct &func_attrs, env &env);

  // For throwaway `func_attrs` (e.g. higher-order functor construction)
  FunctorBase::Ptr CreateFunctor(SVStruct &&func_attrs, env &env);

protected:

  // Noncopyable
  Registry(const Registry&) = delete;
  Registry &operator=(Registry&) = delete;

  std::string name_;

  typedef
    std::unordered_map<std::string, decltype(&detail::CreateFunctor<>)>
    AliasToFunctorFactoryMap;

  AliasToFunctorFactoryMap alias_to_factory_;

  FunctorBase::Ptr CreateFunctor(
        const std::string &alias,
        SVStruct &func_attrs,
        env &env,
        const std::string *name=nullptr);

};

} /* namespace fli */
} /* namespace ok */

#endif /* OK_FLI_REGISTRY_HPP_ */
