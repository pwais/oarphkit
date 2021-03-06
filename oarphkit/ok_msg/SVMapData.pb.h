// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ok_msg/SVMapData.proto

#ifndef PROTOBUF_ok_5fmsg_2fSVMapData_2eproto__INCLUDED
#define PROTOBUF_ok_5fmsg_2fSVMapData_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
#include "ok_msg/DynamicProto.pb.h"
// @@protoc_insertion_point(includes)

namespace ok_msg {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_ok_5fmsg_2fSVMapData_2eproto();
void protobuf_AssignDesc_ok_5fmsg_2fSVMapData_2eproto();
void protobuf_ShutdownFile_ok_5fmsg_2fSVMapData_2eproto();

class SVMapEntryData;
class SVMapData;

// ===================================================================

class SVMapEntryData : public ::google::protobuf::Message {
 public:
  SVMapEntryData();
  virtual ~SVMapEntryData();

  SVMapEntryData(const SVMapEntryData& from);

  inline SVMapEntryData& operator=(const SVMapEntryData& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const SVMapEntryData& default_instance();

  void Swap(SVMapEntryData* other);

  // implements Message ----------------------------------------------

  SVMapEntryData* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const SVMapEntryData& from);
  void MergeFrom(const SVMapEntryData& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional string key = 1;
  inline bool has_key() const;
  inline void clear_key();
  static const int kKeyFieldNumber = 1;
  inline const ::std::string& key() const;
  inline void set_key(const ::std::string& value);
  inline void set_key(const char* value);
  inline void set_key(const char* value, size_t size);
  inline ::std::string* mutable_key();
  inline ::std::string* release_key();
  inline void set_allocated_key(::std::string* key);

  // required string wrapper_typename = 2;
  inline bool has_wrapper_typename() const;
  inline void clear_wrapper_typename();
  static const int kWrapperTypenameFieldNumber = 2;
  inline const ::std::string& wrapper_typename() const;
  inline void set_wrapper_typename(const ::std::string& value);
  inline void set_wrapper_typename(const char* value);
  inline void set_wrapper_typename(const char* value, size_t size);
  inline ::std::string* mutable_wrapper_typename();
  inline ::std::string* release_wrapper_typename();
  inline void set_allocated_wrapper_typename(::std::string* wrapper_typename);

  // optional bytes value_bytes = 3;
  inline bool has_value_bytes() const;
  inline void clear_value_bytes();
  static const int kValueBytesFieldNumber = 3;
  inline const ::std::string& value_bytes() const;
  inline void set_value_bytes(const ::std::string& value);
  inline void set_value_bytes(const char* value);
  inline void set_value_bytes(const void* value, size_t size);
  inline ::std::string* mutable_value_bytes();
  inline ::std::string* release_value_bytes();
  inline void set_allocated_value_bytes(::std::string* value_bytes);

  // optional .ok_msg.DynamicProto dyndata = 4;
  inline bool has_dyndata() const;
  inline void clear_dyndata();
  static const int kDyndataFieldNumber = 4;
  inline const ::ok_msg::DynamicProto& dyndata() const;
  inline ::ok_msg::DynamicProto* mutable_dyndata();
  inline ::ok_msg::DynamicProto* release_dyndata();
  inline void set_allocated_dyndata(::ok_msg::DynamicProto* dyndata);

  GOOGLE_PROTOBUF_EXTENSION_ACCESSORS(SVMapEntryData)
  // @@protoc_insertion_point(class_scope:ok_msg.SVMapEntryData)
 private:
  inline void set_has_key();
  inline void clear_has_key();
  inline void set_has_wrapper_typename();
  inline void clear_has_wrapper_typename();
  inline void set_has_value_bytes();
  inline void clear_has_value_bytes();
  inline void set_has_dyndata();
  inline void clear_has_dyndata();

  ::google::protobuf::internal::ExtensionSet _extensions_;

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* key_;
  ::std::string* wrapper_typename_;
  ::std::string* value_bytes_;
  ::ok_msg::DynamicProto* dyndata_;
  friend void  protobuf_AddDesc_ok_5fmsg_2fSVMapData_2eproto();
  friend void protobuf_AssignDesc_ok_5fmsg_2fSVMapData_2eproto();
  friend void protobuf_ShutdownFile_ok_5fmsg_2fSVMapData_2eproto();

  void InitAsDefaultInstance();
  static SVMapEntryData* default_instance_;
};
// -------------------------------------------------------------------

class SVMapData : public ::google::protobuf::Message {
 public:
  SVMapData();
  virtual ~SVMapData();

  SVMapData(const SVMapData& from);

  inline SVMapData& operator=(const SVMapData& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const SVMapData& default_instance();

  void Swap(SVMapData* other);

  // implements Message ----------------------------------------------

  SVMapData* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const SVMapData& from);
  void MergeFrom(const SVMapData& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated .ok_msg.SVMapEntryData entry = 1;
  inline int entry_size() const;
  inline void clear_entry();
  static const int kEntryFieldNumber = 1;
  inline const ::ok_msg::SVMapEntryData& entry(int index) const;
  inline ::ok_msg::SVMapEntryData* mutable_entry(int index);
  inline ::ok_msg::SVMapEntryData* add_entry();
  inline const ::google::protobuf::RepeatedPtrField< ::ok_msg::SVMapEntryData >&
      entry() const;
  inline ::google::protobuf::RepeatedPtrField< ::ok_msg::SVMapEntryData >*
      mutable_entry();

  // @@protoc_insertion_point(class_scope:ok_msg.SVMapData)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::RepeatedPtrField< ::ok_msg::SVMapEntryData > entry_;
  friend void  protobuf_AddDesc_ok_5fmsg_2fSVMapData_2eproto();
  friend void protobuf_AssignDesc_ok_5fmsg_2fSVMapData_2eproto();
  friend void protobuf_ShutdownFile_ok_5fmsg_2fSVMapData_2eproto();

  void InitAsDefaultInstance();
  static SVMapData* default_instance_;
};
// ===================================================================


// ===================================================================

// SVMapEntryData

// optional string key = 1;
inline bool SVMapEntryData::has_key() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void SVMapEntryData::set_has_key() {
  _has_bits_[0] |= 0x00000001u;
}
inline void SVMapEntryData::clear_has_key() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void SVMapEntryData::clear_key() {
  if (key_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    key_->clear();
  }
  clear_has_key();
}
inline const ::std::string& SVMapEntryData::key() const {
  // @@protoc_insertion_point(field_get:ok_msg.SVMapEntryData.key)
  return *key_;
}
inline void SVMapEntryData::set_key(const ::std::string& value) {
  set_has_key();
  if (key_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    key_ = new ::std::string;
  }
  key_->assign(value);
  // @@protoc_insertion_point(field_set:ok_msg.SVMapEntryData.key)
}
inline void SVMapEntryData::set_key(const char* value) {
  set_has_key();
  if (key_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    key_ = new ::std::string;
  }
  key_->assign(value);
  // @@protoc_insertion_point(field_set_char:ok_msg.SVMapEntryData.key)
}
inline void SVMapEntryData::set_key(const char* value, size_t size) {
  set_has_key();
  if (key_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    key_ = new ::std::string;
  }
  key_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:ok_msg.SVMapEntryData.key)
}
inline ::std::string* SVMapEntryData::mutable_key() {
  set_has_key();
  if (key_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    key_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:ok_msg.SVMapEntryData.key)
  return key_;
}
inline ::std::string* SVMapEntryData::release_key() {
  clear_has_key();
  if (key_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = key_;
    key_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void SVMapEntryData::set_allocated_key(::std::string* key) {
  if (key_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete key_;
  }
  if (key) {
    set_has_key();
    key_ = key;
  } else {
    clear_has_key();
    key_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:ok_msg.SVMapEntryData.key)
}

// required string wrapper_typename = 2;
inline bool SVMapEntryData::has_wrapper_typename() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void SVMapEntryData::set_has_wrapper_typename() {
  _has_bits_[0] |= 0x00000002u;
}
inline void SVMapEntryData::clear_has_wrapper_typename() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void SVMapEntryData::clear_wrapper_typename() {
  if (wrapper_typename_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    wrapper_typename_->clear();
  }
  clear_has_wrapper_typename();
}
inline const ::std::string& SVMapEntryData::wrapper_typename() const {
  // @@protoc_insertion_point(field_get:ok_msg.SVMapEntryData.wrapper_typename)
  return *wrapper_typename_;
}
inline void SVMapEntryData::set_wrapper_typename(const ::std::string& value) {
  set_has_wrapper_typename();
  if (wrapper_typename_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    wrapper_typename_ = new ::std::string;
  }
  wrapper_typename_->assign(value);
  // @@protoc_insertion_point(field_set:ok_msg.SVMapEntryData.wrapper_typename)
}
inline void SVMapEntryData::set_wrapper_typename(const char* value) {
  set_has_wrapper_typename();
  if (wrapper_typename_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    wrapper_typename_ = new ::std::string;
  }
  wrapper_typename_->assign(value);
  // @@protoc_insertion_point(field_set_char:ok_msg.SVMapEntryData.wrapper_typename)
}
inline void SVMapEntryData::set_wrapper_typename(const char* value, size_t size) {
  set_has_wrapper_typename();
  if (wrapper_typename_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    wrapper_typename_ = new ::std::string;
  }
  wrapper_typename_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:ok_msg.SVMapEntryData.wrapper_typename)
}
inline ::std::string* SVMapEntryData::mutable_wrapper_typename() {
  set_has_wrapper_typename();
  if (wrapper_typename_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    wrapper_typename_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:ok_msg.SVMapEntryData.wrapper_typename)
  return wrapper_typename_;
}
inline ::std::string* SVMapEntryData::release_wrapper_typename() {
  clear_has_wrapper_typename();
  if (wrapper_typename_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = wrapper_typename_;
    wrapper_typename_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void SVMapEntryData::set_allocated_wrapper_typename(::std::string* wrapper_typename) {
  if (wrapper_typename_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete wrapper_typename_;
  }
  if (wrapper_typename) {
    set_has_wrapper_typename();
    wrapper_typename_ = wrapper_typename;
  } else {
    clear_has_wrapper_typename();
    wrapper_typename_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:ok_msg.SVMapEntryData.wrapper_typename)
}

// optional bytes value_bytes = 3;
inline bool SVMapEntryData::has_value_bytes() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void SVMapEntryData::set_has_value_bytes() {
  _has_bits_[0] |= 0x00000004u;
}
inline void SVMapEntryData::clear_has_value_bytes() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void SVMapEntryData::clear_value_bytes() {
  if (value_bytes_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    value_bytes_->clear();
  }
  clear_has_value_bytes();
}
inline const ::std::string& SVMapEntryData::value_bytes() const {
  // @@protoc_insertion_point(field_get:ok_msg.SVMapEntryData.value_bytes)
  return *value_bytes_;
}
inline void SVMapEntryData::set_value_bytes(const ::std::string& value) {
  set_has_value_bytes();
  if (value_bytes_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    value_bytes_ = new ::std::string;
  }
  value_bytes_->assign(value);
  // @@protoc_insertion_point(field_set:ok_msg.SVMapEntryData.value_bytes)
}
inline void SVMapEntryData::set_value_bytes(const char* value) {
  set_has_value_bytes();
  if (value_bytes_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    value_bytes_ = new ::std::string;
  }
  value_bytes_->assign(value);
  // @@protoc_insertion_point(field_set_char:ok_msg.SVMapEntryData.value_bytes)
}
inline void SVMapEntryData::set_value_bytes(const void* value, size_t size) {
  set_has_value_bytes();
  if (value_bytes_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    value_bytes_ = new ::std::string;
  }
  value_bytes_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:ok_msg.SVMapEntryData.value_bytes)
}
inline ::std::string* SVMapEntryData::mutable_value_bytes() {
  set_has_value_bytes();
  if (value_bytes_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    value_bytes_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:ok_msg.SVMapEntryData.value_bytes)
  return value_bytes_;
}
inline ::std::string* SVMapEntryData::release_value_bytes() {
  clear_has_value_bytes();
  if (value_bytes_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = value_bytes_;
    value_bytes_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void SVMapEntryData::set_allocated_value_bytes(::std::string* value_bytes) {
  if (value_bytes_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete value_bytes_;
  }
  if (value_bytes) {
    set_has_value_bytes();
    value_bytes_ = value_bytes;
  } else {
    clear_has_value_bytes();
    value_bytes_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:ok_msg.SVMapEntryData.value_bytes)
}

// optional .ok_msg.DynamicProto dyndata = 4;
inline bool SVMapEntryData::has_dyndata() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void SVMapEntryData::set_has_dyndata() {
  _has_bits_[0] |= 0x00000008u;
}
inline void SVMapEntryData::clear_has_dyndata() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void SVMapEntryData::clear_dyndata() {
  if (dyndata_ != NULL) dyndata_->::ok_msg::DynamicProto::Clear();
  clear_has_dyndata();
}
inline const ::ok_msg::DynamicProto& SVMapEntryData::dyndata() const {
  // @@protoc_insertion_point(field_get:ok_msg.SVMapEntryData.dyndata)
  return dyndata_ != NULL ? *dyndata_ : *default_instance_->dyndata_;
}
inline ::ok_msg::DynamicProto* SVMapEntryData::mutable_dyndata() {
  set_has_dyndata();
  if (dyndata_ == NULL) dyndata_ = new ::ok_msg::DynamicProto;
  // @@protoc_insertion_point(field_mutable:ok_msg.SVMapEntryData.dyndata)
  return dyndata_;
}
inline ::ok_msg::DynamicProto* SVMapEntryData::release_dyndata() {
  clear_has_dyndata();
  ::ok_msg::DynamicProto* temp = dyndata_;
  dyndata_ = NULL;
  return temp;
}
inline void SVMapEntryData::set_allocated_dyndata(::ok_msg::DynamicProto* dyndata) {
  delete dyndata_;
  dyndata_ = dyndata;
  if (dyndata) {
    set_has_dyndata();
  } else {
    clear_has_dyndata();
  }
  // @@protoc_insertion_point(field_set_allocated:ok_msg.SVMapEntryData.dyndata)
}

// -------------------------------------------------------------------

// SVMapData

// repeated .ok_msg.SVMapEntryData entry = 1;
inline int SVMapData::entry_size() const {
  return entry_.size();
}
inline void SVMapData::clear_entry() {
  entry_.Clear();
}
inline const ::ok_msg::SVMapEntryData& SVMapData::entry(int index) const {
  // @@protoc_insertion_point(field_get:ok_msg.SVMapData.entry)
  return entry_.Get(index);
}
inline ::ok_msg::SVMapEntryData* SVMapData::mutable_entry(int index) {
  // @@protoc_insertion_point(field_mutable:ok_msg.SVMapData.entry)
  return entry_.Mutable(index);
}
inline ::ok_msg::SVMapEntryData* SVMapData::add_entry() {
  // @@protoc_insertion_point(field_add:ok_msg.SVMapData.entry)
  return entry_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::ok_msg::SVMapEntryData >&
SVMapData::entry() const {
  // @@protoc_insertion_point(field_list:ok_msg.SVMapData.entry)
  return entry_;
}
inline ::google::protobuf::RepeatedPtrField< ::ok_msg::SVMapEntryData >*
SVMapData::mutable_entry() {
  // @@protoc_insertion_point(field_mutable_list:ok_msg.SVMapData.entry)
  return &entry_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace ok_msg

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_ok_5fmsg_2fSVMapData_2eproto__INCLUDED
