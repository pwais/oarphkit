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

#include "ok/fli/Runtime/Session.hpp"

#include <fstream>

#include "ok/fli/fsl/FSLRegistry.hpp"
#include "ok/SerializationUtils/PBUtils.hpp"

namespace ok { namespace fli {

bool Session::ExecFliSpec(const std::string &path) {
  #if OK_ENABLE_PROTOBUF
  auto e_msg = PBFactory::LoadFromPath<ok_msg::EnvState>(path);
  if (e_msg) {
    OKLOG_DEBUG(
      "Session applying pb env: \n" << PBFactory::AsTextFormatString(*e_msg));
    return ExecFliSpec(*e_msg);
  }
  #endif

  OKLOG_ERROR("Don't know how to apply env from " << path);
  return false;
}

bool Session::ExecFliSpecFromTxt(const std::string &spec) {
  #if OK_ENABLE_PROTOBUF
  auto e_msg = PBFactory::LoadFromString<ok_msg::EnvState>(spec);
  if (e_msg) {
    OKLOG_DEBUG("Session applying pb env: \n" << spec);
    return ExecFliSpec(*e_msg);
  }
  #endif

  OKLOG_ERROR("Don't know how to apply env " << spec);
  return false;
}

bool Session::ExecFliSpec(ok_msg::EnvState &e_msg) {
  if (!OKASSERT_CHECK(GetEnv(), "Session does not (yet) wrap an env")) {
    return false;
  }

  auto &e = *GetEnv();

  {
    TimePoint start_apply;
    env from_spec;
    if (!OKASSERT_CHECK(
           from_spec.FromPBMessage(e_msg),
           "Failed to decode given env")) {
      return false;
    }

    e.Union(std::move(from_spec));
    OKLOG_ELAPSED(start_apply, "env decode & apply");
  }

  // Run main if there is one
  if (e.HasMain()) {
    TimePoint start_main;
    bool success = e.RunMain();
    OKLOG_ELAPSED(start_main, "env.main");
    return success;
  }

  return true;
}

Session::UPtr Session::CreateStandardSession() {
  Session::UPtr s(new Session());
  s->InitEmptyEnv();
  s->GetEnv()->GetRegistryRef().UnionWith(*fsl::CreateFSLRegistry());
  return s;
}

bool Session::SaveTextToPath(const std::string &path) {
#if OK_ENABLE_PROTOBUF

  std::ofstream out(path, std::ios::out | std::ios::binary);
  if (!OKASSERT_CHECK(out.good(), "Bad destination: " + path)) {
    return false;
  }

  if (!OKASSERT_CHECK(GetEnv(), "Session does not (yet) wrap an env")) {
    return false;
  }

  auto &env = *GetEnv();

  if (!OKASSERT_CHECK(env.EncodeAllFunctors(), "Error encoding functors")) {
    return false;
  }

  ok_msg::EnvState e_msg;
  if (!OKASSERT_CHECK(env.ToPBMessage(e_msg), "Error encoding env.")) {
    return false;
  }

  // TODO FUTURE: support other text encodings
  std::string pb_txt = PBFactory::AsTextFormatString(e_msg, true);
  if (!OKASSERT_CHECK(!pb_txt.empty(), "Error encoding env to text")) {
    return false;
  }

  out << pb_txt;
  out.close();

  OKLOG("Wrote env to " << path);
  return true;

#else /* no protobuf!  */

  OKLOG_ERROR("This feature requires protobuf");
  return false;

#endif /* OK_ENABLE_PROTOBUF */
}

bool Session::SavePBToPath(const std::string &path) {
#if OK_ENABLE_PROTOBUF

  if (!OKASSERT_CHECK(GetEnv(), "Session does not (yet) wrap an env")) {
    return false;
  }

  auto &env = *GetEnv();

  if (!OKASSERT_CHECK(env.EncodeAllFunctors(), "Error encoding functors")) {
    return false;
  }

  ok_msg::EnvState e_msg;
  if (!OKASSERT_CHECK(env.ToPBMessage(e_msg), "Error encoding env.")) {
    return false;
  }

  if (!OKASSERT_CHECK(
         PBFactory::SaveBinToPath(e_msg, path),
         "Error writing env to disk.")) {
    return false;
  }

  OKLOG("Wrote env to " << path);
  return true;

#else /* no protobuf!  */

  OKLOG_ERROR("This feature requires protobuf");
  return false;

#endif /* OK_ENABLE_PROTOBUF */
}

} /* namespace fli */
} /* namespace ok */

