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
#ifndef OK_FLI_FSL_WHILE_HPP_
#define OK_FLI_FSL_WHILE_HPP_

#include "ok/fli/Core/Functors/Sink.hpp"
#include "ok/fli/Core/Functors/Source.hpp"
#include "ok/fli/Core/Functors/Subroutine.hpp"

namespace ok { namespace fli { namespace fsl {

// Run a Subroutine so long as a condition is true
class While : public Subroutine {
public:

  // == Build & Run ==========================================================

  /*
   * While() must have two functors:
   *  * A `condition` to evaluate per-loop; a Source<bool>.
   *  * A `body` Subroutine (that only modifies internal or env state).
   *     The `body` may actually have non-void output type, but the returned
   *     value is ignored.
   *
   * On compile-time error:
   *  * Returns false; may throw (see OKFLiConfig)
   *  * Leaves While() untouched
   *  * Arguments (e.g. unique_ptrs) may get destroyed
   */

  bool SetCondition(FunctorBase::Ptr &&f);
  bool SetBody(FunctorBase::Ptr &&f);

  // Run the loop to exhaustion
  virtual void Call() override;

  // == Utils ================================================================

  // By default: while(false) { void(); }
  While()
    : condition_(MakeOwned(new FalseCondition())),
      body_(MakeOwned(new Subroutine()))
  { }

  virtual ~While() { }

  typedef TUPtr<While> Ptr;

  virtual std::string RegistryAlias() const { return "fsl::While"; }

  static Ptr CreateEmpty() {
    return MakeOwned(new While());
  }

  virtual std::string ToString() const;

  bool FromEncoded(env &env, SVStruct &m) override;
  bool ToEncoded(env &env, SVStruct &m) override;

  // == Default Sub-Functors =================================================

  class FalseCondition : public Source<bool> {
  public:
    FalseCondition() { }
    virtual ~FalseCondition() { }

    virtual OutTypePtr Call() override {
      static bool result = false;
      return MakeUnOwned(&result);
    }

    virtual std::string ToString() const {
      return "FalseCondition(" + GetSignature() + ")";
    }
  };

  class ForN : public Source<bool> {
  public:
    inline ForN() noexcept : i_(0), n_(0) { }
    inline explicit ForN(uint64_t n) noexcept : i_(0), n_(n) { }
    virtual ~ForN() { }

    virtual std::string RegistryAlias() const { return "fsl::While::ForN"; }

    inline static Ptr Create(uint64_t n) {
      return MakeOwned(new ForN(n));
    }

    inline virtual OutTypePtr Call() override {
      result_ = i_++ < n_;
      return MakeUnOwned(&result_);
    }

    virtual std::string ToString() const;

    bool FromEncoded(env &env, SVStruct &m) override;
    bool ToEncoded(env &env, SVStruct &m) override;

  protected:
    uint64_t i_;
    uint64_t n_;
    bool result_;
  };


protected:
  FunctorBase::Ptr condition_;
  FunctorBase::Ptr body_;
};



} /* namespace fsl */
} /* namespace fli */
} /* namespace ok */

#endif /* OK_FLI_FSL_WHILE_HPP_ */
