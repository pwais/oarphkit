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

#include "ok/fli/fsl/FSLRegistry.hpp"

#include "ok/fli/fsl/Composition.hpp"
#include "ok/fli/fsl/FSLShell.hpp"
#include "ok/fli/fsl/Hash.hpp"
#include "ok/fli/fsl/OarphSay.hpp"
#include "ok/fli/fsl/StdinSource.hpp"
#include "ok/fli/fsl/StdoutSink.hpp"
#include "ok/fli/fsl/Stream.hpp"
#include "ok/fli/fsl/While.hpp"

namespace ok { namespace fli { namespace fsl {

Registry::UPtr CreateFSLRegistry() {
  Registry::UPtr r(new Registry());
  r->SetName("fsl");

  r->Register<Composition>();
  r->Register<FSLShell>();
  r->Register<Hash>();
  r->Register<OarphSayXform>();
  r->Register<StdinSource>();
  r->Register<StdoutSink>();
  r->Register<Stream>();
  r->Register<While>();
  r->Register<While::ForN>();

  return r;
}

} /* namespace fsl */
} /* namespace fli */
} /* namespace ok */


