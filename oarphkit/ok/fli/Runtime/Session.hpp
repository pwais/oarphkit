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

#ifndef OK_FLI_RUNTIME_SESSION_HPP_
#define OK_FLI_RUNTIME_SESSION_HPP_

#include "ok/fli/Runtime/env.hpp"

namespace ok { namespace fli {

class Session final {
public:
  typedef std::unique_ptr<Session> UPtr;
  ~Session() { }

  // Session is only movable
  inline Session() noexcept : env_(NullTUPtr<env>()) { }
  Session(const Session &) = delete;
  Session &operator=(const Session &) = delete;
  Session(Session &&) = default;
  Session &operator=(Session &&) = default;

  inline static Session WrappingEnv(env *e) {
    Session s;
    s.WrapEnv(MakeUnOwned(e));
    return s;
  }

  inline void WrapEnv(env::tptr &&env) { env_ = std::move(env); }

  inline void InitEmptyEnv() { env_ = MakeOwned(new env()); }

  inline env /* weak */ *GetEnv() { return env_.get(); }

  bool ExecFliSpec(const std::string &path);
  bool ExecFliSpecFromTxt(const std::string &spec);
  bool ExecFliSpec(ok_msg::EnvState &e_msg);

  bool SaveTextToPath(const std::string &path);
  bool SavePBToPath(const std::string &path);

  static Session::UPtr CreateStandardSession();

protected:
  env::tptr env_;

  /**
   * todo delets
   * Invocation : Subroutine {
   *
   * Call() {
   *   f = env.getfunc()
   *   res = f->Call(localdata or null)
   *   if name for res, env[name] = res
   *   else
   *   env[anon.result.x] = res
   * }
   *
   * }
   *
   * bool Session.Apply(EnvSpec)
   *
   * bool Session.Eval(FliSpec)
   * bool Session.RunSubroutine(functor varname) -- CT check subroutine
   *
   *
   * topk use case:
   *  flictl --env envspec.prototxt [--env e2 --env e3]
   *              --run main [--run f2 --run f3]
   *    spec:
   *      func {
   *        alias: composition
   *        name: "print top k"
   *        func {
   *          alias: TopKSink (pub histogram / ordered map str -> count)
   *          TopKSinkInit {
   *            k : 10
   *          }  --> XForm<T, SVStruct>, publishes SVStruct with histogram
   *        func {
   *          alias: SVStructAttrToString (SVStruct -> str)
   *        }
   *        func {
   *          alias: stdoutsnk
   *        }
   *      }
   *      func {
   *        alias: stream
   *        ** name: main ***
   *        src: func { alias: stdinsrc }
   *        snk: func { name: "print top k" }
   *      }
   *
   *  Session will Apply() all envspec files then RunSubroutine(main)
   *
   *  FLiCV:
   *
   *
   *
   *  image resize use case. needs:
   *   * image resize base in FliCV
   *   * params, ser, etc for the functor
   *   * input: captain proto src, files src, pb src... Cassie Vede src
   *   * output: same as above
   *   *
   *
   *
   */

};

} /* namespace fli */
} /* namespace ok */

#endif /* OK_FLI_RUNTIME_SESSION_HPP_ */
