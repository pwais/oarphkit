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
#ifndef OK_TEST_SINGLETONSTREAM_HPP_
#define OK_TEST_SINGLETONSTREAM_HPP_

#include <sstream>
#include <string>

namespace ok_test {

// A simple singleton wrapping a stringstream
struct SStreamSingleton {
  std::stringstream ss;

  SStreamSingleton() { }

  static std::stringstream &GetSS() {
    static SStreamSingleton inst;
    return inst.ss;
  }

  static std::string GetStrAndReset() {
    auto s = GetSS().str();
    GetSS() = std::stringstream();
    return s;
  }
};

} /* namespace ok_test */

#endif /* OK_TEST_SINGLETONSTREAM_HPP_ */
