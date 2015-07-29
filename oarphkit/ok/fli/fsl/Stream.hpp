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
#ifndef OK_FLI_FSL_STREAM_HPP_
#define OK_FLI_FSL_STREAM_HPP_

#include "ok/fli/Core/Functors/Sink.hpp"
#include "ok/fli/Core/Functors/Source.hpp"
#include "ok/fli/Core/Functors/Subroutine.hpp"

namespace ok { namespace fli { namespace fsl {

// TODO: need to finish this (tests)

// Consume from a Source until it's empty (i.e. it returns null)
class Stream : public Subroutine {
public:

  // == Build & Run ==========================================================

  /*
   * Stream() must have two functors:
   *  * A source that provides a stream of T.  `src` must return
   *     nullptr / rt_datumptr() to signal end-of-stream.
   *  * A sink that consumes T (and may may actually have
   *     non-void output type, but the returned value is ignored).
   *
   * On compile-time error:
   *  * Returns false; may throw (see OKFLiConfig)
   *  * Leaves Stream() untouched
   *  * Arguments (e.g. unique_ptrs) may get destroyed
   */

  bool SetSource(FunctorBase::Ptr &&f);
  bool SetSink(FunctorBase::Ptr &&f);
    // TODO FUTURE: Allow Stream to accept a Composition b/t src & snk

  // Run the stream to exhaustion; i.e. until Source returns null
  virtual void Call() override;

  // == Utils ================================================================

  // By default: does nothing on Call()
  Stream()
    : src_(NullTUPtr<Source<Void>>()),
      snk_(NullTUPtr<Sink<Void>>())
  { }

  virtual ~Stream() { }

  typedef TUPtr<Stream> Ptr;

  virtual std::string RegistryAlias() const { return "fsl::Stream"; }

  static Ptr CreateEmpty() {
    return MakeOwned(new Stream());
  }

  bool FromEncoded(env &env, SVStruct &m) override;
  bool ToEncoded(env &env, SVStruct &m) override;

  virtual std::string ToString() const;

protected:
  FunctorBase::Ptr src_;
  FunctorBase::Ptr snk_;
};

} /* namespace fsl */
} /* namespace fli */
} /* namespace ok */

#endif /* OK_FLI_FSL_STREAM_HPP_ */
