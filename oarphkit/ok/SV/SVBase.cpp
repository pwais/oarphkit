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

#include "ok/SV/SVBase.hpp"

#if OK_ENABLE_PROTOBUF
#include "ok_msg/SVMapData.pb.h"
#endif

namespace ok { namespace detail {

void SVBase::CopyRegisteredEntriesTo(SVBase &other) {
  if (OK_LIKELY(wname_to_factory_)) {
    other.wname_to_factory_ =
      WrappernameToFactoryMapPtr(
        new WrappernameToFactoryMap(
          *wname_to_factory_));
  }
}

// =========================================================================
// PB Serializable

#if OK_ENABLE_PROTOBUF

bool SVBase::ToPBMessage(ok_msg::SVMapData &pb_sv_map) {
  auto ref_map = GetSVRefMap();
  for (auto &entry : ref_map) {
    const auto &key = entry.first;
    auto &val = entry.second;

    ok_msg::SVMapEntryData pb_entry;

    bool success = false;
    if (val->IsIPBSerializable()) {
      success = val->ToSVMapEntryData(pb_entry);
    } else {
      // TODO: handle byte-serializable
      OKLOG("Don't know how to encode key " << val->ClassKey() << "; skipping");
      continue;
    }

    if (!OKASSERT_CHECK(
          success,
          "Failed trying to encode val " + val->ToString() +
            "for key: " + std::string(val->ClassKey()) +
            ", encoded message is INCOMPLETE")) {
      return false;
    }

    // SVMap will want to save the key and thus provide a non-null ptr
    if (key) {
      pb_entry.set_key(*key);
    }

    // 'Move' (er, Swap) into place
    pb_sv_map.add_entry()->Swap(&pb_entry);
  }
  return true;
}

bool SVBase::FromPBMessage(ok_msg::SVMapData &pb_sv_map) {
  for (size_t i = 0; i < pb_sv_map.entry_size(); ++i) {
    auto *pb_entry = pb_sv_map.mutable_entry(i);
    const auto &wname = pb_entry->wrapper_typename();

    if (!SupportsEntry(wname)) {
      OKLOG("Don't know how to decode " + wname + "; skipping");
      continue;
    }

    auto entry_factory = GetEntryFactory(wname);
    auto valwrapper = entry_factory(*pb_entry);
    if (!OKASSERT_CHECK(
          valwrapper,
          "Failed trying to decode wrapper " + wname +
            ", decoded message is INCOMPLETE")) {
      return false;
    }

    AssignFromPBEntry(*pb_entry, std::move(valwrapper));
  }

  return true;
}

#endif /* OK_ENABLE_PROTOBUF */

} /* namespace detail */
} /* namespace ok */
