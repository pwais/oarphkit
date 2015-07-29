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
#ifndef OK_SERIALIZATIONUTILS_IPBSERIALIZABLE_HPP_
#define OK_SERIALIZATIONUTILS_IPBSERIALIZABLE_HPP_

#include "ok/Config.hpp"

namespace ok {

/**
 * Interface for an object that can be serialized to/from a protobuf message.
 *
 * To use this interface:
 *  * Define a protobuf message type MessageT
 *  * Forward-declare MessageT in your object's header file and subclass
 *      from IPBSerializable<IPBSerializable>.
 *  * Be careful to use OK_ENABLE_PROTOBUF guards to protect:
 *     - the #import of the MessageT header
 *     - the /declaration/ of the overridden IPBSerializable methods
 *     - implementation of the overridden IPBSerializable methods
 *  Thus if Protobuf is not available, this interface will compile to no-ops
 *  and error messages.
 */
template <typename MessageT>
class IPBSerializable {
public:

  typedef MessageT MessageType;

  /**
   * An Init method that replaces the current object's contents with
   * those of `m` and return true only upon success.
   *
   * This object could potentially /move/ data from `m` (e.g. via swap),
   * thus invalidating `m`.
   */
  virtual bool FromPBMessage(MessageType &m) {
    if (!OK_ENABLE_PROTOBUF) {
      OKLOG_ERROR("Cannot deserialize without protobuf");
    }
    return false;
  }

  /**
   * Writes this object's state to `m` and returns true on success.
   *
   * This object could potentially /move/ data into `m` (e.g. via swap)
   * and invalidate the current object.
   */
  virtual bool ToPBMessage(MessageType &m) {
    if (!OK_ENABLE_PROTOBUF) {
      OKLOG_ERROR("Cannot serialize without protobuf");
    }
    return false;
  }
};

} /* namespace ok */

#endif /* OK_SERIALIZATIONUTILS_IPBSERIALIZABLE_HPP_ */
