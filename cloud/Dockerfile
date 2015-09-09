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

FROM ubuntu:utopic-20150625

RUN apt-get update
RUN apt-get -y install git build-essential clang llvm lldb-3.4 ccache vim curl wget time cmake dh-autoreconf libc++abi-dev libc++-dev
RUN echo "Need LLVM >= 3.2 for mixed c++11 support" && clang --version

# For >= cmake 3.1
RUN apt-get -y install software-properties-common python-software-properties
RUN add-apt-repository ppa:george-edison55/cmake-3.x 
RUN apt-get update 
RUN apt-get -y upgrade cmake

# For djinni
RUN apt-get install -y openjdk-8-jdk ant
ENV JAVA_HOME /usr/lib/jvm/java-8-openjdk-amd64/
