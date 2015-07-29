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
#ifndef OK_FLI_FSL_RANGE_HPP_
#define OK_FLI_FSL_RANGE_HPP_

#include "ok/fli/Core/Functors/Source.hpp"

namespace ok { namespace fli { namespace fsl {

// Generate a sequence of integers
class Range : public Source<int64_t> {

public:

  // == Build & Run ==========================================================

  /*
   * Range() has params similar to python's range():
   *  * start [default 0] - the first value to generate
   *  * step [default 1] - compute the next value to generate;
   *     computes /nothing/ if step is 0
   *  * limit - stop generating values once the current value
   *     is greater or equal to `limit` (if step is positive;
   *     if step is negative, then stop when current <= limit).
   */

  // By default: an empty range / empty source
  Range()
    : current_(0), step_(1), limit_(0)
  { }

  explicit Range(int64_t limit, int64_t start=0, int64_t step=1)
    : current_(start), step_(step), limit_(limit)
  { }

  inline
  virtual TUPtr<int64_t> Call() {
    if (!OKASSERT_CHECK(step_ != 0, "Oarph!! inf. range hissss")) {
      return NullTUPtr<int64_t>();
    }
    if (step_ > 0 ? current_ >= limit_ : current_ <= limit_) {
      return NullTUPtr<int64_t>();
    }
    emitted_ = current_;
    current_ += step_;
    return MakeUnOwned<int64_t>(&emitted_);
  }

  // == Utils ================================================================

  typedef TUPtr<Range> Ptr;

  virtual ~Range() { }

  virtual std::string ToString() const;

  virtual std::string RegistryAlias() const { return "fsl::Range"; }

  bool FromEncoded(env &env, SVStruct &m) override;
  bool ToEncoded(env &env, SVStruct &m) override;

protected:
  int64_t current_, step_, limit_, emitted_;
};

} /* namespace fsl */
} /* namespace fli */
} /* namespace ok */

#endif /* OK_FLI_FSL_RANGE_HPP_ */
