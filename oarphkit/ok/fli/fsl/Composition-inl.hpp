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
#ifndef OK_FLI_COMBINATORS_COMPOSITION_INL_HPP_
#define OK_FLI_COMBINATORS_COMPOSITION_INL_HPP_

#ifndef OK_FLI_COMBINATORS_COMPOSITION_HPP_
#error "Include Composition.hpp instead"
#endif

namespace ok { namespace fli { namespace fsl {

inline
bool Composition::Prepend(FunctorBase::Ptr &&f) {
  OK_FLI_RET_FALSE_IF_NOT(CTCheckHeadWith(*f));

  ftors_.push_front(std::move(f));
  return true;
}

inline
bool Composition::Append(FunctorBase::Ptr &&f) {
  OK_FLI_RET_FALSE_IF_NOT(CTCheckTailWith(*f));

  ftors_.push_back(std::move(f));
  return true;
}

template <typename RangeIterableT>
bool Composition::PrependAll(RangeIterableT &&fs) {
  std::list<FunctorBase::Ptr> accpeted_ftors;
  for (auto &f : fs) {
    OKASSERT_FATAL(f, "Null functor!");
    if (OK_LIKELY(!accpeted_ftors.empty())) {
      OK_FLI_RET_FALSE_IF_NOT(CTCheckCanCompose(*f, *accpeted_ftors.back()));
    }
    accpeted_ftors.push_back(std::move(f));
  }

  OK_FLI_RET_FALSE_IF_NOT(CTCheckHeadWith(*accpeted_ftors.back()));

  // Accepted! prepend!
  ftors_.splice(ftors_.begin(), accpeted_ftors);
  return true;
}

template <typename RangeIterableT>
bool Composition::AppendAll(RangeIterableT &&fs) {
  std::list<FunctorBase::Ptr> accpeted_ftors;
  for (auto &f : fs) {
    OKASSERT_FATAL(f, "Null functor!");
    if (OK_UNLIKELY(accpeted_ftors.empty())) {
      OK_FLI_RET_FALSE_IF_NOT(CTCheckTailWith(*f));
    } else {
      OK_FLI_RET_FALSE_IF_NOT(CTCheckCanCompose(*f, *accpeted_ftors.back()));
    }
    accpeted_ftors.push_back(std::move(f));
  }

  // Accepted! append!
  ftors_.splice(ftors_.end(), accpeted_ftors);
  return true;
}

inline
bool Composition::CTCheckHeadWith(const FunctorBase &f) const {

  if (ftors_.empty()) { return true; }

  return OK_FLI_CT_CHECK(
    f.OutType(),
    ftors_.front()->InType(),
    "Given function of signature \n" +
      f.GetSignature() + " " +
      "with RETURN TYPE INCOMPATIBLE with head function: \n" +
      ftors_.front()->GetSignature() + "\n\n" +
      "head: " + ftors_.front()->ToString() + "\n" +
      "given: " + f.ToString());
}

inline
bool Composition::CTCheckTailWith(const FunctorBase &f) const {

  if (ftors_.empty()) { return true; }

  return OK_FLI_CT_CHECK(
    ftors_.back()->OutType(),
    f.InType(),
    "Given function of signature \n" +
      f.GetSignature() + " " +
      "with INPUT TYPE INCOMPATIBLE with tail function: \n" +
      ftors_.back()->GetSignature() + "\n\n" +
      "tail: " + ftors_.back()->ToString() + "\n" +
      "given: " + f.ToString());
}

inline
bool Composition::CTCheckCanCompose(
                      const FunctorBase &outer,
                      const FunctorBase &inner) {

  return OK_FLI_CT_CHECK(
    inner.OutType(),
    outer.InType(),
    "Cannot compose function of signature \n" +
      inner.GetSignature() + " " +
      "inside\n" +
      outer.GetSignature() + "\n" +
      "inner RETURN TYPE INCOMPATIBLE with outer INPUT TYPE" +
      "\n\n" +
      "inner: " + inner.ToString() + "\n" +
      "outer: " + outer.ToString());
}

template <typename InT, typename OutT>
TUPtr<OutT>
Composition::Call(InT *arg) {
  if (OK_UNLIKELY(ftors_.empty())) { return NullTUPtr<OutT>(); }
  return Call(rt_datumptr::Reference(arg)).template ToTPtr<OutT>();
}

} /* namespace fsl */
} /* namespace fli */
} /* namespace ok */


#endif /* OK_FLI_COMBINATORS_COMPOSITION_INL_HPP_ */
