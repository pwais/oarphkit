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

#include "ok/fli/fsl/Composition.hpp"

#include "ok/fli/Utils/Serialization/PBEncodingUtils.hpp"
#include "ok/Stringify.hpp"

namespace ok { namespace fli { namespace fsl {

rt_datumptr Composition::Call(rt_datumptr in) {
  rt_datumptr temp = std::move(in);
  for (auto &f : ftors_) {
    temp = f->Call(std::move(temp));
  }
  return temp;
}

std::string Composition::ToString() const {
  std::stringstream ss;
  ss << "Composition(" << GetSignature() << ":";
  for (const auto &fp : ftors_) {
    ss << fp->ToString();
    if (OK_UNLIKELY(fp != ftors_.back())) { ss << "->"; }
  }
  ss << ")";
  return ss.str();
}

bool Composition::FromEncoded(env &env, SVStruct &m) {
  bool decoded = false;

  // = BEGIN PB ==============================================================
  #if OK_ENABLE_PROTOBUF
  decoded |=
    DecodeFromPBFunc(
      env,
      m,
      ok_msg::CompositionInit::composition,
      [&] (ok_msg::Func &spec) {
        auto func_specs =
            spec.MutableExtension(
                ok_msg::CompositionInit::composition)->mutable_func();
        for (size_t i = 0; i < func_specs->size(); ++i) {
          auto f = env.CreateFunctor(FuncAttrsFromPB(*func_specs->Mutable(i)));
          if (!OKASSERT_CHECK(
                 f.get(),
                 "Could not create a functor for \n" +
                 PBFactory::AsTextFormatString(func_specs->Get(i)))) {
            return false;
          }

          this->Append(std::move(f));
        }

        return true;
      });
  #endif
  // = END PB ================================================================

  #if OK_ENABLE_CAPNP
  // TODO (CAPNP)
  #endif

  return
    OKASSERT_CHECK(decoded, "Don't know how to decode from " + m.ToString());
}

bool Composition::ToEncoded(env &env, SVStruct &m) {

  bool encoded = false;

  // = BEGIN PB ==============================================================
  #if OK_ENABLE_PROTOBUF
  encoded |=
    EncodeToPBFunc(
      env,
      m,
      RegistryAlias(),
      [&] (ok_msg::Func &spec) {
        auto comp_spec_funcs =
            spec.MutableExtension(
                ok_msg::CompositionInit::composition)->mutable_func();

        // Encode functors as references to env variables
        auto f_to_name = env.GetFunctorToName();
        for (const auto &f_ptr : ftors_) {
          auto f_key = f_ptr.get();
          if (!OKASSERT_CHECK(
                 f_to_name.find(f_key) != f_to_name.end(),
                 "Can't find referenced functor " + Stringify((void *)f_key) +
                 " in env Functors: " + Stringify(f_to_name))) {
            return false;
          }

          auto varname = f_to_name.at(f_key);
          auto func_spec = comp_spec_funcs->Add();
          func_spec->set_name(varname);
        }
        return true;
      });
  #endif
  // = END PB ================================================================

  #if OK_ENABLE_CAPNP
  // TODO (CAPNP)
  #endif

  return
    OKASSERT_CHECK(encoded, "Don't know how to encode to " + m.ToString());
}

} /* namespace fsl */
} /* namespace fli */
} /* namespace ok */
