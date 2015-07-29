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

#include "gtest/gtest.h"

#include "ok/SerializationUtils/IPBSerializable.hpp"

TEST(IPBSerializableTest, TestBasic) {
  struct msg { };

  class Oarph : public ok::IPBSerializable<msg> {
  public:
    Oarph() { }
    virtual ~Oarph() { }

    // Instructions are for subclasses to impl overrides with guards
    #if OK_ENABLE_PROTOBUF
    bool FromPBMessage(MessageType &m) override { return true; }
    bool ToPBMessage(MessageType &m) override { return true; }
    #endif
  };

  Oarph o;
  msg m;
  #if OK_ENABLE_PROTOBUF
    // Protobuf is avail, so superclass should be invoked
    EXPECT_TRUE(o.FromPBMessage(m));
    EXPECT_TRUE(o.ToPBMessage(m));
  #else
    // Protobuf is not avail, so base IPBSerializable should be invoked
    EXPECT_FALSE(o.FromPBMessage(m));
    EXPECT_FALSE(o.ToPBMessage(m));
  #endif
}
