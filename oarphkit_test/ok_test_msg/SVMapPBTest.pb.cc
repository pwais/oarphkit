// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: SVMapPBTest.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "SVMapPBTest.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace ok_test_msg {

namespace {

const ::google::protobuf::Descriptor* SmallTestMessage_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  SmallTestMessage_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_SVMapPBTest_2eproto() {
  protobuf_AddDesc_SVMapPBTest_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "SVMapPBTest.proto");
  GOOGLE_CHECK(file != NULL);
  SmallTestMessage_descriptor_ = file->message_type(0);
  static const int SmallTestMessage_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SmallTestMessage, vstr_),
  };
  SmallTestMessage_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      SmallTestMessage_descriptor_,
      SmallTestMessage::default_instance_,
      SmallTestMessage_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SmallTestMessage, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SmallTestMessage, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(SmallTestMessage));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_SVMapPBTest_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    SmallTestMessage_descriptor_, &SmallTestMessage::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_SVMapPBTest_2eproto() {
  delete SmallTestMessage::default_instance_;
  delete SmallTestMessage_reflection_;
}

void protobuf_AddDesc_SVMapPBTest_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::ok_msg::protobuf_AddDesc_ok_5fmsg_2fSVMapData_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\021SVMapPBTest.proto\022\013ok_test_msg\032\026ok_msg"
    "/SVMapData.proto\" \n\020SmallTestMessage\022\014\n\004"
    "vstr\030\001 \001(\t:_\n SmallTestMessageSVMapEntry"
    "_field\022\026.ok_msg.SVMapEntryData\030d \001(\0132\035.o"
    "k_test_msg.SmallTestMessageB.\n\032org.oarph"
    "kittest.msg.protoB\013SVMapPBTestH\001\200\001\000", 235);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "SVMapPBTest.proto", &protobuf_RegisterTypes);
  SmallTestMessage::default_instance_ = new SmallTestMessage();
  ::google::protobuf::internal::ExtensionSet::RegisterMessageExtension(
    &::ok_msg::SVMapEntryData::default_instance(),
    100, 11, false, false,
    &::ok_test_msg::SmallTestMessage::default_instance());
  SmallTestMessage::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_SVMapPBTest_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_SVMapPBTest_2eproto {
  StaticDescriptorInitializer_SVMapPBTest_2eproto() {
    protobuf_AddDesc_SVMapPBTest_2eproto();
  }
} static_descriptor_initializer_SVMapPBTest_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int SmallTestMessage::kVstrFieldNumber;
#endif  // !_MSC_VER

SmallTestMessage::SmallTestMessage()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:ok_test_msg.SmallTestMessage)
}

void SmallTestMessage::InitAsDefaultInstance() {
}

SmallTestMessage::SmallTestMessage(const SmallTestMessage& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:ok_test_msg.SmallTestMessage)
}

void SmallTestMessage::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  vstr_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

SmallTestMessage::~SmallTestMessage() {
  // @@protoc_insertion_point(destructor:ok_test_msg.SmallTestMessage)
  SharedDtor();
}

void SmallTestMessage::SharedDtor() {
  if (vstr_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete vstr_;
  }
  if (this != default_instance_) {
  }
}

void SmallTestMessage::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* SmallTestMessage::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return SmallTestMessage_descriptor_;
}

const SmallTestMessage& SmallTestMessage::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_SVMapPBTest_2eproto();
  return *default_instance_;
}

SmallTestMessage* SmallTestMessage::default_instance_ = NULL;

SmallTestMessage* SmallTestMessage::New() const {
  return new SmallTestMessage;
}

void SmallTestMessage::Clear() {
  if (has_vstr()) {
    if (vstr_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
      vstr_->clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool SmallTestMessage::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:ok_test_msg.SmallTestMessage)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional string vstr = 1;
      case 1: {
        if (tag == 10) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_vstr()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->vstr().data(), this->vstr().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "vstr");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:ok_test_msg.SmallTestMessage)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:ok_test_msg.SmallTestMessage)
  return false;
#undef DO_
}

void SmallTestMessage::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:ok_test_msg.SmallTestMessage)
  // optional string vstr = 1;
  if (has_vstr()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->vstr().data(), this->vstr().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "vstr");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->vstr(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:ok_test_msg.SmallTestMessage)
}

::google::protobuf::uint8* SmallTestMessage::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:ok_test_msg.SmallTestMessage)
  // optional string vstr = 1;
  if (has_vstr()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->vstr().data(), this->vstr().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "vstr");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->vstr(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:ok_test_msg.SmallTestMessage)
  return target;
}

int SmallTestMessage::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional string vstr = 1;
    if (has_vstr()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->vstr());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void SmallTestMessage::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const SmallTestMessage* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const SmallTestMessage*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void SmallTestMessage::MergeFrom(const SmallTestMessage& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_vstr()) {
      set_vstr(from.vstr());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void SmallTestMessage::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void SmallTestMessage::CopyFrom(const SmallTestMessage& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool SmallTestMessage::IsInitialized() const {

  return true;
}

void SmallTestMessage::Swap(SmallTestMessage* other) {
  if (other != this) {
    std::swap(vstr_, other->vstr_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata SmallTestMessage::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = SmallTestMessage_descriptor_;
  metadata.reflection = SmallTestMessage_reflection_;
  return metadata;
}

::google::protobuf::internal::ExtensionIdentifier< ::ok_msg::SVMapEntryData,
    ::google::protobuf::internal::MessageTypeTraits< ::ok_test_msg::SmallTestMessage >, 11, false >
  SmallTestMessageSVMapEntry_field(kSmallTestMessageSVMapEntryFieldFieldNumber, ::ok_test_msg::SmallTestMessage::default_instance());

// @@protoc_insertion_point(namespace_scope)

}  // namespace ok_test_msg

// @@protoc_insertion_point(global_scope)
