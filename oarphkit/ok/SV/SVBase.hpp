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

#ifndef OK_SV_SVBASE_HPP_
#define OK_SV_SVBASE_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "ok/Common.hpp"
#include "ok/SerializationUtils/IPBSerializable.hpp"
#include "ok/SV/SVCommon.hpp"

namespace ok_msg { class SVMapData; }
namespace ok_msg { class SVMapEntryData; }
  // Forwards, avoid include .pb.h

namespace ok { namespace detail {

// ===========================================================================
// SVMap/SVStruct Shared Base

class SVBase : public IPBSerializable<ok_msg::SVMapData> {
public:

  SVBase() = default;
  virtual ~SVBase() { }

  // Noncopyable and movable-only because it owns its values
  SVBase(const SVBase&) = delete;
  SVBase &operator=(const SVBase&) = delete;
  SVBase(SVBase&&) = default;
  SVBase &operator=(SVBase &&) = default;

  // Register an entry type for deserialization support
  template <typename EntryT>
  inline
  void RegisterEntry() {
    // Lazy Init
    if (OK_UNLIKELY(!wname_to_factory_)) {
      wname_to_factory_ =
          WrappernameToFactoryMapPtr(new WrappernameToFactoryMap());
    }
    (*wname_to_factory_)[EntryT::Key()] = &EntryT::FromSVMapEntryData;
  }

  void CopyRegisteredEntriesTo(SVBase &other);

  inline virtual bool IsEmpty() const { return true; }

  virtual std::string ToString() const { return "SVBase()"; }

  // =========================================================================
  // PB Serializable

  #if OK_ENABLE_PROTOBUF
  bool ToPBMessage(ok_msg::SVMapData &pb_sv_map) override;
  bool FromPBMessage(ok_msg::SVMapData &pb_sv_map) override;
  #endif /* OK_ENABLE_PROTOBUF */

  virtual void AssignFromPBEntry(
      ok_msg::SVMapEntryData &pb_entry,
      detail::SVEntryWrapperBase::UPtr &&valwrapper) {
    /* base assigns nothing */
  }

protected:
//  std::unordered_map<KeyType, detail::SVEntryWrapperBase::UPtr> entrymap_;

//  typedef
//     std::unordered_map<std::string, detail::SVEntryWrapperBase::UPtr>
//  static std::string MapToString(
//      const  &m) {
//    std::stringstream ss;
//    for (const auto &entry : m) {
//      ss << entry.first << " -> " << entry.second->ToString() << "\n\n";
//    }
//    return ss.str();
//  }

  // =========================================================================
  // Serialization Utils

  typedef
    std::vector<
      std::pair<
        const std::string /* weak */ *,
        detail::SVEntryWrapperBase /* weak */ *>>
    SVRefMap;

  virtual SVRefMap GetSVRefMap() const { return SVRefMap(); }


  typedef
    decltype(&::ok::detail::SVEntryWrapperBase::FromSVMapEntryData)
    ValWrapperFactoryFunc;

  typedef
    std::unordered_map<std::string, ValWrapperFactoryFunc>
    WrappernameToFactoryMap;

  typedef std::unique_ptr<WrappernameToFactoryMap> WrappernameToFactoryMapPtr;

  WrappernameToFactoryMapPtr wname_to_factory_; // Lazy!

  inline
  bool SupportsEntry(const std::string &wname) const {
    if (!wname_to_factory_) { return false; }
    return wname_to_factory_->find(wname) != wname_to_factory_->end();
  }

  inline
  ValWrapperFactoryFunc GetEntryFactory(const std::string &wname) {
    return (*wname_to_factory_)[wname];
  }

};

} /* namespace detail */
} /* namespace ok */

#endif /* OK_SV_SVBASE_HPP_ */
