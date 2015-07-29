// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ok_msg/DynamicProto.proto

#ifndef PROTOBUF_ok_5fmsg_2fDynamicProto_2eproto__INCLUDED
#define PROTOBUF_ok_5fmsg_2fDynamicProto_2eproto__INCLUDED

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
// @@protoc_insertion_point(includes)

namespace ok_msg {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_ok_5fmsg_2fDynamicProto_2eproto();
void protobuf_AssignDesc_ok_5fmsg_2fDynamicProto_2eproto();
void protobuf_ShutdownFile_ok_5fmsg_2fDynamicProto_2eproto();

class DynamicProto;

// ===================================================================

class DynamicProto : public ::google::protobuf::Message {
 public:
  DynamicProto();
  virtual ~DynamicProto();

  DynamicProto(const DynamicProto& from);

  inline DynamicProto& operator=(const DynamicProto& from) {
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
  static const DynamicProto& default_instance();

  void Swap(DynamicProto* other);

  // implements Message ----------------------------------------------

  DynamicProto* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const DynamicProto& from);
  void MergeFrom(const DynamicProto& from);
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

  // optional string vStr = 1;
  inline bool has_vstr() const;
  inline void clear_vstr();
  static const int kVStrFieldNumber = 1;
  inline const ::std::string& vstr() const;
  inline void set_vstr(const ::std::string& value);
  inline void set_vstr(const char* value);
  inline void set_vstr(const char* value, size_t size);
  inline ::std::string* mutable_vstr();
  inline ::std::string* release_vstr();
  inline void set_allocated_vstr(::std::string* vstr);

  // optional bytes vBytes = 2;
  inline bool has_vbytes() const;
  inline void clear_vbytes();
  static const int kVBytesFieldNumber = 2;
  inline const ::std::string& vbytes() const;
  inline void set_vbytes(const ::std::string& value);
  inline void set_vbytes(const char* value);
  inline void set_vbytes(const void* value, size_t size);
  inline ::std::string* mutable_vbytes();
  inline ::std::string* release_vbytes();
  inline void set_allocated_vbytes(::std::string* vbytes);

  // optional sint64 vInt = 3;
  inline bool has_vint() const;
  inline void clear_vint();
  static const int kVIntFieldNumber = 3;
  inline ::google::protobuf::int64 vint() const;
  inline void set_vint(::google::protobuf::int64 value);

  // optional uint64 vUInt = 4;
  inline bool has_vuint() const;
  inline void clear_vuint();
  static const int kVUIntFieldNumber = 4;
  inline ::google::protobuf::uint64 vuint() const;
  inline void set_vuint(::google::protobuf::uint64 value);

  // optional bool vBool = 5;
  inline bool has_vbool() const;
  inline void clear_vbool();
  static const int kVBoolFieldNumber = 5;
  inline bool vbool() const;
  inline void set_vbool(bool value);

  // optional float vFloat = 6;
  inline bool has_vfloat() const;
  inline void clear_vfloat();
  static const int kVFloatFieldNumber = 6;
  inline float vfloat() const;
  inline void set_vfloat(float value);

  // optional double vDouble = 7;
  inline bool has_vdouble() const;
  inline void clear_vdouble();
  static const int kVDoubleFieldNumber = 7;
  inline double vdouble() const;
  inline void set_vdouble(double value);

  // repeated sint64 vsInt = 8 [packed = true];
  inline int vsint_size() const;
  inline void clear_vsint();
  static const int kVsIntFieldNumber = 8;
  inline ::google::protobuf::int64 vsint(int index) const;
  inline void set_vsint(int index, ::google::protobuf::int64 value);
  inline void add_vsint(::google::protobuf::int64 value);
  inline const ::google::protobuf::RepeatedField< ::google::protobuf::int64 >&
      vsint() const;
  inline ::google::protobuf::RepeatedField< ::google::protobuf::int64 >*
      mutable_vsint();

  // repeated uint64 vsUInt = 9 [packed = true];
  inline int vsuint_size() const;
  inline void clear_vsuint();
  static const int kVsUIntFieldNumber = 9;
  inline ::google::protobuf::uint64 vsuint(int index) const;
  inline void set_vsuint(int index, ::google::protobuf::uint64 value);
  inline void add_vsuint(::google::protobuf::uint64 value);
  inline const ::google::protobuf::RepeatedField< ::google::protobuf::uint64 >&
      vsuint() const;
  inline ::google::protobuf::RepeatedField< ::google::protobuf::uint64 >*
      mutable_vsuint();

  // repeated sint32 vsInt32 = 10 [packed = true];
  inline int vsint32_size() const;
  inline void clear_vsint32();
  static const int kVsInt32FieldNumber = 10;
  inline ::google::protobuf::int32 vsint32(int index) const;
  inline void set_vsint32(int index, ::google::protobuf::int32 value);
  inline void add_vsint32(::google::protobuf::int32 value);
  inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
      vsint32() const;
  inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
      mutable_vsint32();

  // repeated uint32 vsUInt32 = 11 [packed = true];
  inline int vsuint32_size() const;
  inline void clear_vsuint32();
  static const int kVsUInt32FieldNumber = 11;
  inline ::google::protobuf::uint32 vsuint32(int index) const;
  inline void set_vsuint32(int index, ::google::protobuf::uint32 value);
  inline void add_vsuint32(::google::protobuf::uint32 value);
  inline const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >&
      vsuint32() const;
  inline ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >*
      mutable_vsuint32();

  // repeated float vsFloat = 12 [packed = true];
  inline int vsfloat_size() const;
  inline void clear_vsfloat();
  static const int kVsFloatFieldNumber = 12;
  inline float vsfloat(int index) const;
  inline void set_vsfloat(int index, float value);
  inline void add_vsfloat(float value);
  inline const ::google::protobuf::RepeatedField< float >&
      vsfloat() const;
  inline ::google::protobuf::RepeatedField< float >*
      mutable_vsfloat();

  // repeated double vsDouble = 13 [packed = true];
  inline int vsdouble_size() const;
  inline void clear_vsdouble();
  static const int kVsDoubleFieldNumber = 13;
  inline double vsdouble(int index) const;
  inline void set_vsdouble(int index, double value);
  inline void add_vsdouble(double value);
  inline const ::google::protobuf::RepeatedField< double >&
      vsdouble() const;
  inline ::google::protobuf::RepeatedField< double >*
      mutable_vsdouble();

  // repeated string vsStr = 14;
  inline int vsstr_size() const;
  inline void clear_vsstr();
  static const int kVsStrFieldNumber = 14;
  inline const ::std::string& vsstr(int index) const;
  inline ::std::string* mutable_vsstr(int index);
  inline void set_vsstr(int index, const ::std::string& value);
  inline void set_vsstr(int index, const char* value);
  inline void set_vsstr(int index, const char* value, size_t size);
  inline ::std::string* add_vsstr();
  inline void add_vsstr(const ::std::string& value);
  inline void add_vsstr(const char* value);
  inline void add_vsstr(const char* value, size_t size);
  inline const ::google::protobuf::RepeatedPtrField< ::std::string>& vsstr() const;
  inline ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_vsstr();

  // repeated string k = 15;
  inline int k_size() const;
  inline void clear_k();
  static const int kKFieldNumber = 15;
  inline const ::std::string& k(int index) const;
  inline ::std::string* mutable_k(int index);
  inline void set_k(int index, const ::std::string& value);
  inline void set_k(int index, const char* value);
  inline void set_k(int index, const char* value, size_t size);
  inline ::std::string* add_k();
  inline void add_k(const ::std::string& value);
  inline void add_k(const char* value);
  inline void add_k(const char* value, size_t size);
  inline const ::google::protobuf::RepeatedPtrField< ::std::string>& k() const;
  inline ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_k();

  // repeated .ok_msg.DynamicProto v = 16;
  inline int v_size() const;
  inline void clear_v();
  static const int kVFieldNumber = 16;
  inline const ::ok_msg::DynamicProto& v(int index) const;
  inline ::ok_msg::DynamicProto* mutable_v(int index);
  inline ::ok_msg::DynamicProto* add_v();
  inline const ::google::protobuf::RepeatedPtrField< ::ok_msg::DynamicProto >&
      v() const;
  inline ::google::protobuf::RepeatedPtrField< ::ok_msg::DynamicProto >*
      mutable_v();

  // @@protoc_insertion_point(class_scope:ok_msg.DynamicProto)
 private:
  inline void set_has_vstr();
  inline void clear_has_vstr();
  inline void set_has_vbytes();
  inline void clear_has_vbytes();
  inline void set_has_vint();
  inline void clear_has_vint();
  inline void set_has_vuint();
  inline void clear_has_vuint();
  inline void set_has_vbool();
  inline void clear_has_vbool();
  inline void set_has_vfloat();
  inline void clear_has_vfloat();
  inline void set_has_vdouble();
  inline void clear_has_vdouble();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* vstr_;
  ::std::string* vbytes_;
  ::google::protobuf::int64 vint_;
  ::google::protobuf::uint64 vuint_;
  bool vbool_;
  float vfloat_;
  double vdouble_;
  ::google::protobuf::RepeatedField< ::google::protobuf::int64 > vsint_;
  mutable int _vsint_cached_byte_size_;
  ::google::protobuf::RepeatedField< ::google::protobuf::uint64 > vsuint_;
  mutable int _vsuint_cached_byte_size_;
  ::google::protobuf::RepeatedField< ::google::protobuf::int32 > vsint32_;
  mutable int _vsint32_cached_byte_size_;
  ::google::protobuf::RepeatedField< ::google::protobuf::uint32 > vsuint32_;
  mutable int _vsuint32_cached_byte_size_;
  ::google::protobuf::RepeatedField< float > vsfloat_;
  mutable int _vsfloat_cached_byte_size_;
  ::google::protobuf::RepeatedField< double > vsdouble_;
  mutable int _vsdouble_cached_byte_size_;
  ::google::protobuf::RepeatedPtrField< ::std::string> vsstr_;
  ::google::protobuf::RepeatedPtrField< ::std::string> k_;
  ::google::protobuf::RepeatedPtrField< ::ok_msg::DynamicProto > v_;
  friend void  protobuf_AddDesc_ok_5fmsg_2fDynamicProto_2eproto();
  friend void protobuf_AssignDesc_ok_5fmsg_2fDynamicProto_2eproto();
  friend void protobuf_ShutdownFile_ok_5fmsg_2fDynamicProto_2eproto();

  void InitAsDefaultInstance();
  static DynamicProto* default_instance_;
};
// ===================================================================


// ===================================================================

// DynamicProto

// optional string vStr = 1;
inline bool DynamicProto::has_vstr() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void DynamicProto::set_has_vstr() {
  _has_bits_[0] |= 0x00000001u;
}
inline void DynamicProto::clear_has_vstr() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void DynamicProto::clear_vstr() {
  if (vstr_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    vstr_->clear();
  }
  clear_has_vstr();
}
inline const ::std::string& DynamicProto::vstr() const {
  // @@protoc_insertion_point(field_get:ok_msg.DynamicProto.vStr)
  return *vstr_;
}
inline void DynamicProto::set_vstr(const ::std::string& value) {
  set_has_vstr();
  if (vstr_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    vstr_ = new ::std::string;
  }
  vstr_->assign(value);
  // @@protoc_insertion_point(field_set:ok_msg.DynamicProto.vStr)
}
inline void DynamicProto::set_vstr(const char* value) {
  set_has_vstr();
  if (vstr_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    vstr_ = new ::std::string;
  }
  vstr_->assign(value);
  // @@protoc_insertion_point(field_set_char:ok_msg.DynamicProto.vStr)
}
inline void DynamicProto::set_vstr(const char* value, size_t size) {
  set_has_vstr();
  if (vstr_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    vstr_ = new ::std::string;
  }
  vstr_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:ok_msg.DynamicProto.vStr)
}
inline ::std::string* DynamicProto::mutable_vstr() {
  set_has_vstr();
  if (vstr_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    vstr_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:ok_msg.DynamicProto.vStr)
  return vstr_;
}
inline ::std::string* DynamicProto::release_vstr() {
  clear_has_vstr();
  if (vstr_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = vstr_;
    vstr_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void DynamicProto::set_allocated_vstr(::std::string* vstr) {
  if (vstr_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete vstr_;
  }
  if (vstr) {
    set_has_vstr();
    vstr_ = vstr;
  } else {
    clear_has_vstr();
    vstr_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:ok_msg.DynamicProto.vStr)
}

// optional bytes vBytes = 2;
inline bool DynamicProto::has_vbytes() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void DynamicProto::set_has_vbytes() {
  _has_bits_[0] |= 0x00000002u;
}
inline void DynamicProto::clear_has_vbytes() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void DynamicProto::clear_vbytes() {
  if (vbytes_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    vbytes_->clear();
  }
  clear_has_vbytes();
}
inline const ::std::string& DynamicProto::vbytes() const {
  // @@protoc_insertion_point(field_get:ok_msg.DynamicProto.vBytes)
  return *vbytes_;
}
inline void DynamicProto::set_vbytes(const ::std::string& value) {
  set_has_vbytes();
  if (vbytes_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    vbytes_ = new ::std::string;
  }
  vbytes_->assign(value);
  // @@protoc_insertion_point(field_set:ok_msg.DynamicProto.vBytes)
}
inline void DynamicProto::set_vbytes(const char* value) {
  set_has_vbytes();
  if (vbytes_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    vbytes_ = new ::std::string;
  }
  vbytes_->assign(value);
  // @@protoc_insertion_point(field_set_char:ok_msg.DynamicProto.vBytes)
}
inline void DynamicProto::set_vbytes(const void* value, size_t size) {
  set_has_vbytes();
  if (vbytes_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    vbytes_ = new ::std::string;
  }
  vbytes_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:ok_msg.DynamicProto.vBytes)
}
inline ::std::string* DynamicProto::mutable_vbytes() {
  set_has_vbytes();
  if (vbytes_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    vbytes_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:ok_msg.DynamicProto.vBytes)
  return vbytes_;
}
inline ::std::string* DynamicProto::release_vbytes() {
  clear_has_vbytes();
  if (vbytes_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = vbytes_;
    vbytes_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void DynamicProto::set_allocated_vbytes(::std::string* vbytes) {
  if (vbytes_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete vbytes_;
  }
  if (vbytes) {
    set_has_vbytes();
    vbytes_ = vbytes;
  } else {
    clear_has_vbytes();
    vbytes_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:ok_msg.DynamicProto.vBytes)
}

// optional sint64 vInt = 3;
inline bool DynamicProto::has_vint() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void DynamicProto::set_has_vint() {
  _has_bits_[0] |= 0x00000004u;
}
inline void DynamicProto::clear_has_vint() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void DynamicProto::clear_vint() {
  vint_ = GOOGLE_LONGLONG(0);
  clear_has_vint();
}
inline ::google::protobuf::int64 DynamicProto::vint() const {
  // @@protoc_insertion_point(field_get:ok_msg.DynamicProto.vInt)
  return vint_;
}
inline void DynamicProto::set_vint(::google::protobuf::int64 value) {
  set_has_vint();
  vint_ = value;
  // @@protoc_insertion_point(field_set:ok_msg.DynamicProto.vInt)
}

// optional uint64 vUInt = 4;
inline bool DynamicProto::has_vuint() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void DynamicProto::set_has_vuint() {
  _has_bits_[0] |= 0x00000008u;
}
inline void DynamicProto::clear_has_vuint() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void DynamicProto::clear_vuint() {
  vuint_ = GOOGLE_ULONGLONG(0);
  clear_has_vuint();
}
inline ::google::protobuf::uint64 DynamicProto::vuint() const {
  // @@protoc_insertion_point(field_get:ok_msg.DynamicProto.vUInt)
  return vuint_;
}
inline void DynamicProto::set_vuint(::google::protobuf::uint64 value) {
  set_has_vuint();
  vuint_ = value;
  // @@protoc_insertion_point(field_set:ok_msg.DynamicProto.vUInt)
}

// optional bool vBool = 5;
inline bool DynamicProto::has_vbool() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void DynamicProto::set_has_vbool() {
  _has_bits_[0] |= 0x00000010u;
}
inline void DynamicProto::clear_has_vbool() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void DynamicProto::clear_vbool() {
  vbool_ = false;
  clear_has_vbool();
}
inline bool DynamicProto::vbool() const {
  // @@protoc_insertion_point(field_get:ok_msg.DynamicProto.vBool)
  return vbool_;
}
inline void DynamicProto::set_vbool(bool value) {
  set_has_vbool();
  vbool_ = value;
  // @@protoc_insertion_point(field_set:ok_msg.DynamicProto.vBool)
}

// optional float vFloat = 6;
inline bool DynamicProto::has_vfloat() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void DynamicProto::set_has_vfloat() {
  _has_bits_[0] |= 0x00000020u;
}
inline void DynamicProto::clear_has_vfloat() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void DynamicProto::clear_vfloat() {
  vfloat_ = 0;
  clear_has_vfloat();
}
inline float DynamicProto::vfloat() const {
  // @@protoc_insertion_point(field_get:ok_msg.DynamicProto.vFloat)
  return vfloat_;
}
inline void DynamicProto::set_vfloat(float value) {
  set_has_vfloat();
  vfloat_ = value;
  // @@protoc_insertion_point(field_set:ok_msg.DynamicProto.vFloat)
}

// optional double vDouble = 7;
inline bool DynamicProto::has_vdouble() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void DynamicProto::set_has_vdouble() {
  _has_bits_[0] |= 0x00000040u;
}
inline void DynamicProto::clear_has_vdouble() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void DynamicProto::clear_vdouble() {
  vdouble_ = 0;
  clear_has_vdouble();
}
inline double DynamicProto::vdouble() const {
  // @@protoc_insertion_point(field_get:ok_msg.DynamicProto.vDouble)
  return vdouble_;
}
inline void DynamicProto::set_vdouble(double value) {
  set_has_vdouble();
  vdouble_ = value;
  // @@protoc_insertion_point(field_set:ok_msg.DynamicProto.vDouble)
}

// repeated sint64 vsInt = 8 [packed = true];
inline int DynamicProto::vsint_size() const {
  return vsint_.size();
}
inline void DynamicProto::clear_vsint() {
  vsint_.Clear();
}
inline ::google::protobuf::int64 DynamicProto::vsint(int index) const {
  // @@protoc_insertion_point(field_get:ok_msg.DynamicProto.vsInt)
  return vsint_.Get(index);
}
inline void DynamicProto::set_vsint(int index, ::google::protobuf::int64 value) {
  vsint_.Set(index, value);
  // @@protoc_insertion_point(field_set:ok_msg.DynamicProto.vsInt)
}
inline void DynamicProto::add_vsint(::google::protobuf::int64 value) {
  vsint_.Add(value);
  // @@protoc_insertion_point(field_add:ok_msg.DynamicProto.vsInt)
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int64 >&
DynamicProto::vsint() const {
  // @@protoc_insertion_point(field_list:ok_msg.DynamicProto.vsInt)
  return vsint_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int64 >*
DynamicProto::mutable_vsint() {
  // @@protoc_insertion_point(field_mutable_list:ok_msg.DynamicProto.vsInt)
  return &vsint_;
}

// repeated uint64 vsUInt = 9 [packed = true];
inline int DynamicProto::vsuint_size() const {
  return vsuint_.size();
}
inline void DynamicProto::clear_vsuint() {
  vsuint_.Clear();
}
inline ::google::protobuf::uint64 DynamicProto::vsuint(int index) const {
  // @@protoc_insertion_point(field_get:ok_msg.DynamicProto.vsUInt)
  return vsuint_.Get(index);
}
inline void DynamicProto::set_vsuint(int index, ::google::protobuf::uint64 value) {
  vsuint_.Set(index, value);
  // @@protoc_insertion_point(field_set:ok_msg.DynamicProto.vsUInt)
}
inline void DynamicProto::add_vsuint(::google::protobuf::uint64 value) {
  vsuint_.Add(value);
  // @@protoc_insertion_point(field_add:ok_msg.DynamicProto.vsUInt)
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::uint64 >&
DynamicProto::vsuint() const {
  // @@protoc_insertion_point(field_list:ok_msg.DynamicProto.vsUInt)
  return vsuint_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::uint64 >*
DynamicProto::mutable_vsuint() {
  // @@protoc_insertion_point(field_mutable_list:ok_msg.DynamicProto.vsUInt)
  return &vsuint_;
}

// repeated sint32 vsInt32 = 10 [packed = true];
inline int DynamicProto::vsint32_size() const {
  return vsint32_.size();
}
inline void DynamicProto::clear_vsint32() {
  vsint32_.Clear();
}
inline ::google::protobuf::int32 DynamicProto::vsint32(int index) const {
  // @@protoc_insertion_point(field_get:ok_msg.DynamicProto.vsInt32)
  return vsint32_.Get(index);
}
inline void DynamicProto::set_vsint32(int index, ::google::protobuf::int32 value) {
  vsint32_.Set(index, value);
  // @@protoc_insertion_point(field_set:ok_msg.DynamicProto.vsInt32)
}
inline void DynamicProto::add_vsint32(::google::protobuf::int32 value) {
  vsint32_.Add(value);
  // @@protoc_insertion_point(field_add:ok_msg.DynamicProto.vsInt32)
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
DynamicProto::vsint32() const {
  // @@protoc_insertion_point(field_list:ok_msg.DynamicProto.vsInt32)
  return vsint32_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
DynamicProto::mutable_vsint32() {
  // @@protoc_insertion_point(field_mutable_list:ok_msg.DynamicProto.vsInt32)
  return &vsint32_;
}

// repeated uint32 vsUInt32 = 11 [packed = true];
inline int DynamicProto::vsuint32_size() const {
  return vsuint32_.size();
}
inline void DynamicProto::clear_vsuint32() {
  vsuint32_.Clear();
}
inline ::google::protobuf::uint32 DynamicProto::vsuint32(int index) const {
  // @@protoc_insertion_point(field_get:ok_msg.DynamicProto.vsUInt32)
  return vsuint32_.Get(index);
}
inline void DynamicProto::set_vsuint32(int index, ::google::protobuf::uint32 value) {
  vsuint32_.Set(index, value);
  // @@protoc_insertion_point(field_set:ok_msg.DynamicProto.vsUInt32)
}
inline void DynamicProto::add_vsuint32(::google::protobuf::uint32 value) {
  vsuint32_.Add(value);
  // @@protoc_insertion_point(field_add:ok_msg.DynamicProto.vsUInt32)
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >&
DynamicProto::vsuint32() const {
  // @@protoc_insertion_point(field_list:ok_msg.DynamicProto.vsUInt32)
  return vsuint32_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >*
DynamicProto::mutable_vsuint32() {
  // @@protoc_insertion_point(field_mutable_list:ok_msg.DynamicProto.vsUInt32)
  return &vsuint32_;
}

// repeated float vsFloat = 12 [packed = true];
inline int DynamicProto::vsfloat_size() const {
  return vsfloat_.size();
}
inline void DynamicProto::clear_vsfloat() {
  vsfloat_.Clear();
}
inline float DynamicProto::vsfloat(int index) const {
  // @@protoc_insertion_point(field_get:ok_msg.DynamicProto.vsFloat)
  return vsfloat_.Get(index);
}
inline void DynamicProto::set_vsfloat(int index, float value) {
  vsfloat_.Set(index, value);
  // @@protoc_insertion_point(field_set:ok_msg.DynamicProto.vsFloat)
}
inline void DynamicProto::add_vsfloat(float value) {
  vsfloat_.Add(value);
  // @@protoc_insertion_point(field_add:ok_msg.DynamicProto.vsFloat)
}
inline const ::google::protobuf::RepeatedField< float >&
DynamicProto::vsfloat() const {
  // @@protoc_insertion_point(field_list:ok_msg.DynamicProto.vsFloat)
  return vsfloat_;
}
inline ::google::protobuf::RepeatedField< float >*
DynamicProto::mutable_vsfloat() {
  // @@protoc_insertion_point(field_mutable_list:ok_msg.DynamicProto.vsFloat)
  return &vsfloat_;
}

// repeated double vsDouble = 13 [packed = true];
inline int DynamicProto::vsdouble_size() const {
  return vsdouble_.size();
}
inline void DynamicProto::clear_vsdouble() {
  vsdouble_.Clear();
}
inline double DynamicProto::vsdouble(int index) const {
  // @@protoc_insertion_point(field_get:ok_msg.DynamicProto.vsDouble)
  return vsdouble_.Get(index);
}
inline void DynamicProto::set_vsdouble(int index, double value) {
  vsdouble_.Set(index, value);
  // @@protoc_insertion_point(field_set:ok_msg.DynamicProto.vsDouble)
}
inline void DynamicProto::add_vsdouble(double value) {
  vsdouble_.Add(value);
  // @@protoc_insertion_point(field_add:ok_msg.DynamicProto.vsDouble)
}
inline const ::google::protobuf::RepeatedField< double >&
DynamicProto::vsdouble() const {
  // @@protoc_insertion_point(field_list:ok_msg.DynamicProto.vsDouble)
  return vsdouble_;
}
inline ::google::protobuf::RepeatedField< double >*
DynamicProto::mutable_vsdouble() {
  // @@protoc_insertion_point(field_mutable_list:ok_msg.DynamicProto.vsDouble)
  return &vsdouble_;
}

// repeated string vsStr = 14;
inline int DynamicProto::vsstr_size() const {
  return vsstr_.size();
}
inline void DynamicProto::clear_vsstr() {
  vsstr_.Clear();
}
inline const ::std::string& DynamicProto::vsstr(int index) const {
  // @@protoc_insertion_point(field_get:ok_msg.DynamicProto.vsStr)
  return vsstr_.Get(index);
}
inline ::std::string* DynamicProto::mutable_vsstr(int index) {
  // @@protoc_insertion_point(field_mutable:ok_msg.DynamicProto.vsStr)
  return vsstr_.Mutable(index);
}
inline void DynamicProto::set_vsstr(int index, const ::std::string& value) {
  // @@protoc_insertion_point(field_set:ok_msg.DynamicProto.vsStr)
  vsstr_.Mutable(index)->assign(value);
}
inline void DynamicProto::set_vsstr(int index, const char* value) {
  vsstr_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set_char:ok_msg.DynamicProto.vsStr)
}
inline void DynamicProto::set_vsstr(int index, const char* value, size_t size) {
  vsstr_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:ok_msg.DynamicProto.vsStr)
}
inline ::std::string* DynamicProto::add_vsstr() {
  return vsstr_.Add();
}
inline void DynamicProto::add_vsstr(const ::std::string& value) {
  vsstr_.Add()->assign(value);
  // @@protoc_insertion_point(field_add:ok_msg.DynamicProto.vsStr)
}
inline void DynamicProto::add_vsstr(const char* value) {
  vsstr_.Add()->assign(value);
  // @@protoc_insertion_point(field_add_char:ok_msg.DynamicProto.vsStr)
}
inline void DynamicProto::add_vsstr(const char* value, size_t size) {
  vsstr_.Add()->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_add_pointer:ok_msg.DynamicProto.vsStr)
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
DynamicProto::vsstr() const {
  // @@protoc_insertion_point(field_list:ok_msg.DynamicProto.vsStr)
  return vsstr_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
DynamicProto::mutable_vsstr() {
  // @@protoc_insertion_point(field_mutable_list:ok_msg.DynamicProto.vsStr)
  return &vsstr_;
}

// repeated string k = 15;
inline int DynamicProto::k_size() const {
  return k_.size();
}
inline void DynamicProto::clear_k() {
  k_.Clear();
}
inline const ::std::string& DynamicProto::k(int index) const {
  // @@protoc_insertion_point(field_get:ok_msg.DynamicProto.k)
  return k_.Get(index);
}
inline ::std::string* DynamicProto::mutable_k(int index) {
  // @@protoc_insertion_point(field_mutable:ok_msg.DynamicProto.k)
  return k_.Mutable(index);
}
inline void DynamicProto::set_k(int index, const ::std::string& value) {
  // @@protoc_insertion_point(field_set:ok_msg.DynamicProto.k)
  k_.Mutable(index)->assign(value);
}
inline void DynamicProto::set_k(int index, const char* value) {
  k_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set_char:ok_msg.DynamicProto.k)
}
inline void DynamicProto::set_k(int index, const char* value, size_t size) {
  k_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:ok_msg.DynamicProto.k)
}
inline ::std::string* DynamicProto::add_k() {
  return k_.Add();
}
inline void DynamicProto::add_k(const ::std::string& value) {
  k_.Add()->assign(value);
  // @@protoc_insertion_point(field_add:ok_msg.DynamicProto.k)
}
inline void DynamicProto::add_k(const char* value) {
  k_.Add()->assign(value);
  // @@protoc_insertion_point(field_add_char:ok_msg.DynamicProto.k)
}
inline void DynamicProto::add_k(const char* value, size_t size) {
  k_.Add()->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_add_pointer:ok_msg.DynamicProto.k)
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
DynamicProto::k() const {
  // @@protoc_insertion_point(field_list:ok_msg.DynamicProto.k)
  return k_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
DynamicProto::mutable_k() {
  // @@protoc_insertion_point(field_mutable_list:ok_msg.DynamicProto.k)
  return &k_;
}

// repeated .ok_msg.DynamicProto v = 16;
inline int DynamicProto::v_size() const {
  return v_.size();
}
inline void DynamicProto::clear_v() {
  v_.Clear();
}
inline const ::ok_msg::DynamicProto& DynamicProto::v(int index) const {
  // @@protoc_insertion_point(field_get:ok_msg.DynamicProto.v)
  return v_.Get(index);
}
inline ::ok_msg::DynamicProto* DynamicProto::mutable_v(int index) {
  // @@protoc_insertion_point(field_mutable:ok_msg.DynamicProto.v)
  return v_.Mutable(index);
}
inline ::ok_msg::DynamicProto* DynamicProto::add_v() {
  // @@protoc_insertion_point(field_add:ok_msg.DynamicProto.v)
  return v_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::ok_msg::DynamicProto >&
DynamicProto::v() const {
  // @@protoc_insertion_point(field_list:ok_msg.DynamicProto.v)
  return v_;
}
inline ::google::protobuf::RepeatedPtrField< ::ok_msg::DynamicProto >*
DynamicProto::mutable_v() {
  // @@protoc_insertion_point(field_mutable_list:ok_msg.DynamicProto.v)
  return &v_;
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

#endif  // PROTOBUF_ok_5fmsg_2fDynamicProto_2eproto__INCLUDED
