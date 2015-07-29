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

#ifndef OK_FLI_FSL_FSLSHELL_HPP_
#define OK_FLI_FSL_FSLSHELL_HPP_

#include <list>
#include <string>

#include "ok/fli/Core/Functors/Subroutine.hpp"

namespace ok { namespace fli {

struct env; // Forward

namespace fsl {

class FSLShell : public Subroutine {
public:

  // = Command Interface =====================================================

  class ICommand {
  public:
    typedef std::unique_ptr<ICommand> Ptr;

    virtual std::string GetName() { return "ICommand"; }

    virtual std::string GetHelp() { return "Halp! I'm an interface!"; }

    virtual bool Run(FSLShell &sh) { /* NB: Use sh.GetEnv() */ return true; }

    ICommand() = default;
    virtual ~ICommand() { }
  };

  inline env /* weak */ *GetEnv() const { return env_; }

  inline void SetShouldContinueLooping(bool loop) { looping_ = loop; }

  inline void SetShouldBreakOnError(bool br) { break_on_error_ = br; }

  inline std::list<ICommand::Ptr> &GetCommandsRef() { return commands_; }

  template <typename CommandT>
  inline void AddCommand() {
    AddCommand(ICommand::Ptr(new CommandT()));
  }

  inline void AddCommand(ICommand::Ptr &&cmd) {
    commands_.push_back(std::move(cmd));
  }

  inline virtual std::string GetPrompt() const { return "fsl > "; }



  // = Utils =================================================================

  typedef TUPtr<FSLShell> Ptr;

  inline virtual std::string RegistryAlias() const { return "fsl::shell"; }

  FSLShell(); // Includes basic commands
  virtual ~FSLShell() { }

  void Call() override;

  static Ptr Create() { return MakeOwned(new FSLShell()); }

  inline virtual std::string ToString() const {
    return "FSLShell(" + GetSignature() + ")";
  }

  // FSLShell is a pure functor besides a reference to the env
  // upon which it acts
  bool FromEncoded(env &env, SVStruct &m) override {
    env_ = &env;
    return true;
  }

  bool ToEncoded(env &env, SVStruct &m) override { return true; }

protected:

  // Noncopyable
  FSLShell(const FSLShell&) = delete;
  FSLShell &operator=(FSLShell&) = delete;

  bool break_on_error_;
  bool looping_;

  std::list<ICommand::Ptr> commands_;

  env /* weak */ * env_;

};

} /* namespace fsl */
} /* namespace fli */
} /* namespace ok */

#endif /* OK_FLI_FSL_FSLSHELL_HPP_ */
