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

#ifndef OK_SV_SVMAP_HPP_
#define OK_SV_SVMAP_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "ok/SV/SVBase.hpp"

namespace ok {

/**
 * SVMap: A String -> Value map with explicit value types, but no centralized
 * schema.  Similar to a JSON object or a Python dict.
 *
 * SVMap values can share an entry wrapper type (i.e. if they are the
 * same type).
 */
class SVMap final : public detail::SVBase {
public:

  typedef std::shared_ptr<SVMap> SPtr;
  typedef std::unique_ptr<SVMap> UPtr;

  SVMap() = default;
  virtual ~SVMap() { }
  SVMap(SVMap&&) = default;
  SVMap &operator=(SVMap &&) = default;

  inline virtual bool IsEmpty() const override { return entrymap_.empty(); }
  std::string ToString() const override;

  inline bool HasKey(const std::string &k) const {
    return entrymap_.find(k) != entrymap_.end();
  }

  template <typename EntryT>
  void SetKey(const std::string &k, typename EntryT::WrappedType &&val) {
    entrymap_[k] = EntryT::Wrap(std::move(val));
  }

  template <typename EntryT>
  void SetKey(const std::string &k, const typename EntryT::WrappedType &val) {
    SetKey<EntryT>(
        k,
        std::unique_ptr<typename EntryT::WrappedType>(
            new typename EntryT::WrappedType(val)));
  }

  template <typename EntryT>
  void SetKey(
      const std::string &k,
      std::unique_ptr<typename EntryT::WrappedType> &&valptr) {
    entrymap_[k] = EntryT::WrapPtr(std::move(valptr));
  }

  template <typename EntryT>
  void SetKey(const std::string &k, const ByteIArrayUPtr &bytes) {
    entrymap_[k] = EntryT::FromBytes(bytes);
  }

  template <typename EntryT>
  typename EntryT::WrappedType &GetValueRef(const std::string &k) {
    return reinterpret_cast<EntryT *>(entrymap_.at(k).get())->GetValueRef();
  }

  template <typename EntryT>
  const typename EntryT::WrappedType &GetValue(const std::string &k) const {
    return reinterpret_cast<EntryT *>(entrymap_.at(k).get())->GetValue();
  }

  template <typename EntryT>
  std::string GetValueAsString(const std::string &k) const {
    return reinterpret_cast<EntryT *>(entrymap_.at(k).get())->ToString();
  }

  inline
  void ClearKey(const std::string &k) {
    entrymap_.erase(k);
  }

  inline
  std::string GetValueType(const std::string &k) const {
    return std::string(entrymap_.at(k)->ClassKey());
  }

  std::vector<std::string> GetKeys() const;

  inline
  void MoveKeyTo(const std::string &k, SVMap &other) {
    if (OK_LIKELY(HasKey(k))) {
      other.entrymap_[k] = std::move(this->entrymap_.at(k));
      ClearKey(k);
    }
  }

  template <typename ContainerStrT>
  inline
  void MoveKeysTo(const ContainerStrT &keys, SVMap &other) {
    for (const auto &k : keys) {
      MoveKeyTo(k, other);
    }
  }

  template <typename ContainerStrT>
  inline
  SVMap Splice(const ContainerStrT &keys) {
    SVMap s;
    CopyRegisteredEntriesTo(s);
    MoveKeysTo(keys, s);
    return s;
  }

protected:

  std::unordered_map<std::string, detail::SVEntryWrapperBase::UPtr> entrymap_;

  SVRefMap GetSVRefMap() const override;

  #if OK_ENABLE_PROTOBUF

  inline
  void AssignFromPBEntry(
      ok_msg::SVMapEntryData &pb_entry,
      detail::SVEntryWrapperBase::UPtr &&valwrapper) override {
    entrymap_[pb_entry.key()] = std::move(valwrapper);
  }

  #endif /* OK_ENABLE_PROTOBUF */
};





} /* namespace ok */



#endif /* OK_SV_SVMAP_HPP_ */
