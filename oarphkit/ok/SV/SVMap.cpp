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

#include "ok/SV/SVMap.hpp"

#include <sstream>

namespace ok {

std::string SVMap::ToString() const {
  std::stringstream ss;
  ss << "\n=== === SVMap Begin === ===\n\n";
  for (const auto &entry : entrymap_) {
    ss << entry.first << " -> " << entry.second->ToString() << "\n\n";
  }
  ss << "=== === SVMap End === ===\n\n";
  return ss.str();
}

std::vector<std::string> SVMap::GetKeys() const {
  std::vector<std::string> keys;
  keys.reserve(entrymap_.size());
  for (const auto &entry : entrymap_) {
    keys.push_back(entry.first);
  }
  return keys;
}

detail::SVBase::SVRefMap SVMap::GetSVRefMap() const {
  detail::SVBase::SVRefMap ref_map;
  ref_map.reserve(entrymap_.size());
  for (const auto &entry : entrymap_) {
    ref_map.push_back(
      std::make_pair(
        &(entry.first),
        entry.second.get()));
  }
  return ref_map;
}

} /* namespace ok */




// TODO: delete capnp example later
//#include <ostream>
//#if OK_ENABLE_CAPNP
//#include <capnp/serialize.h>
//#include <kj/io.h>
//#include <kj/std/iostream.h>
//#include "ok_msg/SVMapData.capnp.h"
//#endif

//namespace detail {
//
//class SVMapUtils {
//public:
//
//
//#if OK_ENABLE_CAPNP
//
//// Encode the SVMap data `sv_map` using capnp
//template <typename MapT>
//static ByteIArray ToBytesCapnp(const MapT &sv_map) {
//  ::capnp::MallocMessageBuilder message;
//
//  auto svmap_capnp = message.initRoot<ok_msg::capnpm::SVMapData>();
//  auto skeys = svmap_capnp.initKeys(sv_map.size());
//  auto svals = svmap_capnp.initValues(sv_map.size());
//  size_t i = 0;
//  for (const auto &entry : sv_map) {
//    const auto &key = entry.first;
//    auto val_bytes = entry.second->ValToBytes();
//    if (!val_bytes) { continue; }
//
//    // FIXME: believe this may leak: the Data::Builder() does not copy
//    skeys[i] = ::capnp::Text::Builder((char *)key);
//    svals[i] =
//        ::capnp::Data::Builder(
//          (::kj::byte *)val_bytes->Data(),
//          val_bytes->Size());
//  }
//
//  // FIXME use CPUtils
//  std::stringstream ss;
//  ::kj::std::StdOutputStream os(ss);
//  ::capnp::writeMessage(os, message);
//  return ByteIArray::WrappingContainer(ss.str());
//}
//#endif
//
//template <typename MapT>
//static std::string ToString(const MapT &m) {
//  std::stringstream ss;
//  for (const auto &entry : m) {
//    ss << entry.first << " -> " << entry.second->ToString() << "\n\n";
//  }
//  return ss.str();
//}
//
//}; /* class SVMapUtils */
//
//} /* namespace detail */
