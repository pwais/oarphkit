# Copyright 2015 Maintainers of OarphKit
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

@0xa279f89414de71e4;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("ok_msg::capnpm");

# TODO: include capnp java in build
# using Java = import "/capnp/java.capnp";
# $Java.package("org.oarphkit.msg.capnpm");
# $Java.outerClassname("SVMapData");

struct SVMapData {
  keys@0 :List(Text);
  values@1 :List(Data);
}
