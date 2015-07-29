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
#ifndef OK_FLI_COMBINATORS_COMPOSITION_HPP_
#define OK_FLI_COMBINATORS_COMPOSITION_HPP_

#include <list>

#include "ok/fli/Core/Core.hpp"

namespace ok { namespace fli { namespace fsl {

class Composition : public FunctorBase {
public:

  // == Utils ================================================================

  Composition() = default;
  virtual ~Composition() { }

  typedef TUPtr<Composition> Ptr;

  bool FromEncoded(env &env, SVStruct &m) override;
  bool ToEncoded(env &env, SVStruct &m) override;

  static Ptr CreateEmpty() { return MakeOwned(new Composition()); }

  virtual std::string RegistryAlias() const { return "fsl::Composition"; }

  virtual std::string ToString() const;

  // == Build & Run ==========================================================

  /*
   * Append and prepend a function `f` to the composition. Runs
   * compile-time checks.
   *
   * On compile-time error:
   *  * Returns false, but
   *  * May throw (see OKFLiConfig)
   *  * Underlying Composition left untouched
   *  * Arguments (e.g. unique_ptrs) may get destroyed
   *
   * If you want the Composition to own the provided `f`, pass a
   * FunctorBase::Ptr that has a deleter (e.g. from MakeOwned)
   */

  bool Prepend(FunctorBase::Ptr &&f);
  bool Append(FunctorBase::Ptr &&f);

  /**
   * Prepend: f1 -> f2 -> f3 -> ... -> fN -> this
   *         (head / innermost) --> (tail / outermost)
   *
   * Requires O(`fs`) temp space
   */
  template <typename RangeIterableT>
  bool PrependAll(RangeIterableT &&fs);
  //

  /**
   * Append: this -> f1 -> f2 -> f3 -> ... -> fN
   *         (head / innermost) --> (tail / outermost)
   *
   * Requires O(`fs`) temp space
   */
  template <typename RangeIterableT>
  bool AppendAll(RangeIterableT &&fs);

  /**
   * Invoke the composition.  A Composition of zero functions returns
   * nullptr.
   */
  template <typename InT, typename OutT>
  TUPtr<OutT> Call(InT *arg=nullptr);

protected:
  std::list<FunctorBase::Ptr> ftors_;

  inline virtual std::type_index InType() const override {
    return OK_UNLIKELY(ftors_.empty()) ?
        FunctorBase::InType() :
        ftors_.front()->InType();
  }

  inline virtual std::type_index OutType() const override {
    return OK_UNLIKELY(ftors_.empty()) ?
        FunctorBase::OutType() :
        ftors_.back()->OutType();
  }

  virtual rt_datumptr Call(rt_datumptr in) override;

  static bool CTCheckCanCompose(
                const FunctorBase &outer,
                const FunctorBase &inner);
  bool CTCheckHeadWith(const FunctorBase &f) const;
  bool CTCheckTailWith(const FunctorBase &f) const;

};

} /* namespace fsl */
} /* namespace fli */
} /* namespace ok */

#include "ok/fli/fsl/Composition-inl.hpp"

#endif /* OK_FLI_COMBINATORS_COMPOSITION_HPP_ */
