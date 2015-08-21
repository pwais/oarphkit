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

#ifndef OK_SV_SVSTRUCT_HPP_
#define OK_SV_SVSTRUCT_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "ok/SV/SVBase.hpp"

namespace ok {

/**
 * SVStruct: A struct-like object with an explicit (but *decentralized*
 * schema).  Furthermore, fields may be elided completely. Similar to a
 * protocol buffer message.
 *
 * Each SVStruct entry must have a distinct entry wrapper declared;
 * unlike SVMap, SVStruct maintains a bijection between "keys"
 * (i.e. attributes) and declared entries.
 */
class SVStruct final : public detail::SVBase {
public:

  typedef std::shared_ptr<SVStruct> SPtr;
  typedef std::unique_ptr<SVStruct> UPtr;

  SVStruct() = default;
  virtual ~SVStruct() { }
  SVStruct(SVStruct&&) = default;
  SVStruct &operator=(SVStruct &&) = default;

  inline virtual bool IsEmpty() const override { return entrymap_.empty(); }
  std::string ToString() const override;

  template <typename EntryT>
  bool HasAttr() const {
    return entrymap_.find(EntryT::Key()) != entrymap_.end();
  }

  template <typename EntryT>
  void SetAttr(typename EntryT::WrappedType &&val) {
    entrymap_[EntryT::Key()] = EntryT::Wrap(std::move(val));
  }

  template <typename EntryT>
  void SetAttr(const typename EntryT::WrappedType &val) {
    SetAttr<EntryT>(
        std::unique_ptr<typename EntryT::WrappedType>(
            new typename EntryT::WrappedType(val)));
  }

  template <typename EntryT>
  void SetAttr(std::unique_ptr<typename EntryT::WrappedType> &&valptr) {
    entrymap_[EntryT::Key()] = EntryT::WrapPtr(std::move(valptr));
  }

  template <typename EntryT>
  void SetAttr(const ByteIArrayUPtr &bytes) {
    entrymap_[EntryT::Key()] = EntryT::FromBytes(bytes);
  }

  template <typename EntryT>
  typename EntryT::WrappedType &GetAttrRef() {
OKLOG("key add: " << Stringify((void *) EntryT::Key()));
    return
      reinterpret_cast<EntryT *>(
        entrymap_.at(EntryT::Key()).get())->GetValueRef();
  }

  template <typename EntryT>
  const typename EntryT::WrappedType &GetAttr() const {
    return
      reinterpret_cast<EntryT *>(
        entrymap_.at(EntryT::Key()).get())->GetValue();
  }

  template <typename EntryT>
  std::string GetAttrAsString() const {
    return
      reinterpret_cast<EntryT *>(
        entrymap_.at(EntryT::Key()).get())->ToString();
  }

  template <typename EntryT>
  void ClearAttr() {
    entrymap_.erase(EntryT::Key());
  }

  template <typename EntryT>
  void MoveAttrTo(SVStruct &other) {
    if (HasAttr<EntryT>()) {
      other.SetAttr<EntryT>(
        std::move(
          reinterpret_cast<EntryT *>(
            entrymap_.at(EntryT::Key()).get())->valp_));
      ClearAttr<EntryT>();
    }
  }

protected:

  std::unordered_map<const char *, detail::SVEntryWrapperBase::UPtr> entrymap_;
    // TODO: consider using a sparse map

  SVRefMap GetSVRefMap() const override;

  inline
  void AssignFromPBEntry(
      ok_msg::SVMapEntryData &pb_entry,
      detail::SVEntryWrapperBase::UPtr &&valwrapper) override {
    entrymap_[valwrapper->ClassKey()] = std::move(valwrapper);
  }

};

} /* namespace ok */

#endif /* OK_SV_SVSTRUCT_HPP_ */
