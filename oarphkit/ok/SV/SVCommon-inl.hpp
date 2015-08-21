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

#ifndef OK_SV_SVCOMMON_INL_HPP_
#define OK_SV_SVCOMMON_INL_HPP_

#ifndef OK_SV_SVCOMMON_HPP_
#error "Import SVCommon.hpp instead"
#endif

#include <memory>
#include <string>

#include "ok/Common.hpp"
#include "ok/TemplateFu.hpp"
#include "ok/SerializationUtils/IByteArrayable.hpp"
#include "ok/SerializationUtils/IPBSerializable.hpp"

#if OK_ENABLE_PROTOBUF
#include "ok_msg/SVMapData.pb.h"
#else
namespace ok_msg { class SVMapData; }
namespace ok_msg { class SVMapEntryData; }
#endif

namespace ok {

class SVMap; // Forward
class SVStruct; // Forward

template <typename KeyType>
class SVBase;

static const std::string kSVMapOpaque = "[opaque]";

namespace detail {

// ===========================================================================
// Value Wrapper Base Class

class SVEntryWrapperBase {
public:
  virtual ~SVEntryWrapperBase() { }

  typedef std::unique_ptr<SVEntryWrapperBase> UPtr;

  static std::string EntryTypename() noexcept {
    return std::string(Key());
  }

  static constexpr const char* Key() noexcept {
    return "detail::SVEntryWrapperBase";
  }

  /**
   * TODO: the pub methods below should be protected, but we see a
   * weird clang bug when they are (i.e. error, method marked
   * protected... despite the friend class SVStruct
   * declaration). Could be related to:
   *   https://llvm.org/bugs/show_bug.cgi?id=6840
   */

  virtual const char* ClassKey() noexcept {
    return SVEntryWrapperBase::Key();
  }

  static UPtr FromSVMapEntryData(ok_msg::SVMapEntryData &entry) {
    return nullptr;
  }

  virtual std::string ToString() const = 0;

protected:

  friend class SVMap;
  friend class SVStruct;
  friend class SVBase;

  virtual bool IsIPBSerializable() const noexcept { return false; }

  virtual bool ToSVMapEntryData(ok_msg::SVMapEntryData &entry) {
    return false;
  }
};

template <typename T>
inline
std::string NonStringifiable(const T &val) { return kSVMapOpaque; }

// TODO Add IByteArraySerializable feature
//template <typename T>
//inline
//ROByteIArray::UPtr NonByteSerializable(const T &val) { return nullptr; }
//
//template <typename T>
//inline
//std::unique_ptr<T> NonByteDeserializable(const ByteIArray &bytes) { return nullptr; }



// ===========================================================================
// IPBSerializable-related Macros

#define OK_SVMAP_NOT_IPBSERIALIZABLE() \
  inline bool IsIPBSerializable() const noexcept override { return false; }

#if OK_ENABLE_PROTOBUF

  /**
   * This macro declares member functions inside a SVEntryWrapperBase
   * subclass (see OK_DECLARE_SV_ENTRY_IMPL()) to enable a
   * IPBSerializable-type value to (de)serialize from a Protobuf
   * message.  To use:
   *  * You must declare an extension of the SVMapEntryData messsage
   *      in a .proto file that imports SVMapData.proto.  The declared
   *      extension field:
   *        - Must have a type equal to the Message type of your
   *            IPBSerializable class.
   *        - Ideally has a name like "KeyNameSVMapEntry_field",
   *            where "KeyName" is the name of the SVMap Entry class
   *            (i.e. first argument to OK_DECLARE_SV_ENTRY_IMPL())
   *  * You must include the protobuf header file for your protobuf message
   *     in the file where you use this macro (but wrap it in a
   *     OK_ENABLE_PROTOBUF guard if you want your code to compile
   *     without protobuf support). Similarly, you should include a
   *     forward-declaration of the Message type of your
   *     IPBSerializable class inside a !OK_ENABLE_PROTOBUF guard.
   *  * If Protobuf is not available, the members that this macro declares
   *     will no-op and log errors.
   */

  #define OK_SVMAP_IPBSERIALIZABLE(ExtensionName) \
    /* =================================================================== */ \
    /* IPBSerializable Members                                             */ \
    \
    inline bool IsIPBSerializable() const noexcept override { return true; } \
    \
    static ::ok::detail::SVEntryWrapperBase::UPtr FromSVMapEntryData( \
        ::ok_msg::SVMapEntryData &entry) { \
      if (!OKASSERT_CHECK( \
            entry.HasExtension(ExtensionName), \
            "Corrupted message")) { \
        return nullptr; \
      } \
      auto *msg = entry.MutableExtension(ExtensionName); \
      std::unique_ptr<WrappedType> val(new WrappedType()); \
      bool success = val->FromPBMessage(*msg); \
      if (!OKASSERT_CHECK(success, "Failed to decode message")) { \
        return nullptr; \
      } \
      return WrapPtr(std::move(val)); \
    } \
    \
    bool ToSVMapEntryData(::ok_msg::SVMapEntryData &entry) override { \
      entry.set_wrapper_typename(Key()); \
      auto *msg = entry.MutableExtension(ExtensionName); \
      return valp_->ToPBMessage(*msg); \
    }

  #define OK_SVMAP_PBDYNDATA_SERIALIZABLE() \
    /* =================================================================== */ \
    /* IPBSerializable Members                                             */ \
    \
    inline bool IsIPBSerializable() const noexcept override { return true; } \
    \
    static ::ok::detail::SVEntryWrapperBase::UPtr FromSVMapEntryData( \
        ::ok_msg::SVMapEntryData &entry) { \
      static_assert( \
        sizeof(ok_msg::DynamicProto) > 0, \
        "Did you forget to import DynamicProtoUtils?"); \
      if (!OKASSERT_CHECK(entry.has_dyndata(), "Corrupted message")) { \
        return nullptr; \
      } \
      const auto &d = entry.dyndata(); \
      \
      if (!OKASSERT_CHECK( \
            ::ok::DynHasValue<WrappedType>(d), \
            "Message missing expected dyndata")) { \
        return nullptr; \
      }\
      \
      std::unique_ptr<WrappedType> val(new WrappedType()); \
      *val = ::ok::DynGetValue<WrappedType>(d); \
      return WrapPtr(std::move(val)); \
    } \
    \
    bool ToSVMapEntryData(::ok_msg::SVMapEntryData &entry) override { \
      entry.set_wrapper_typename(Key()); \
      ::ok::DynSetValue(*entry.mutable_dyndata(), GetValue()); \
      return true; \
    }

  #define OK_SVMAP_PBDYNARR_SERIALIZABLE() \
    /* =================================================================== */ \
    /* IPBSerializable Members                                             */ \
    \
    inline bool IsIPBSerializable() const noexcept override { return true; } \
    \
    static ::ok::detail::SVEntryWrapperBase::UPtr FromSVMapEntryData( \
        ::ok_msg::SVMapEntryData &entry) { \
      static_assert( \
        sizeof(::ok_msg::DynamicProto) > 0, \
        "Did you forget to import DynamicProtoUtils?"); \
      if (!OKASSERT_CHECK(entry.has_dyndata(), "Corrupted message")) { \
        return nullptr; \
      } \
      const auto &d = entry.dyndata(); \
      \
      if (!OKASSERT_CHECK( \
            !::ok::DynGetArray<WrappedType::ValueType>(d).IsEmpty(), \
            "Message missing expected dyndata array")) { \
        return nullptr; \
      }\
      \
      std::unique_ptr<WrappedType> val(new WrappedType()); \
      *val = ::ok::DynGetArray<WrappedType::ValueType>(d); \
      return WrapPtr(std::move(val)); \
    } \
    \
    bool ToSVMapEntryData(::ok_msg::SVMapEntryData &entry) override { \
      entry.set_wrapper_typename(Key()); \
      ::ok::DynSetArray(*entry.mutable_dyndata(), GetValue()); \
      return true; \
    }

  #define OK_SVMAP_IPB_OF_DYNDATA_SERIALIZABLE() \
    /* =================================================================== */ \
    /* IPBSerializable Members                                             */ \
    \
    inline bool IsIPBSerializable() const noexcept override { return true; } \
    \
    static ::ok::detail::SVEntryWrapperBase::UPtr FromSVMapEntryData( \
        ::ok_msg::SVMapEntryData &entry) { \
      if (!OKASSERT_CHECK(entry.has_dyndata(), "Corrupted message")) { \
        return nullptr; \
      } \
      auto *d = entry.mutable_dyndata(); \
      std::unique_ptr<WrappedType> val(new WrappedType()); \
      bool success = val->FromPBMessage(*d); \
      if (!OKASSERT_CHECK(success, "Failed to decode message")) { \
        return nullptr; \
      } \
      return WrapPtr(std::move(val)); \
    } \
    \
    bool ToSVMapEntryData(::ok_msg::SVMapEntryData &entry) override { \
      entry.set_wrapper_typename(Key()); \
      return valp_->ToPBMessage(*entry.mutable_dyndata()); \
    }

#else /* No protobuf! */

  #define OK_SVMAP_IPBSERIALIZABLE(ExtensionName) \
    /* =================================================================== */ \
    /* IPBSerializable Helpers                                             */ \
    \
    inline bool IsIPBSerializable() const noexcept override { return false; } \
    \
    static std::unique_ptr<typename EntryT::WrappedType> FromSVMapEntryData( \
        ::ok_msg::SVMapEntryData &entry) { \
      OKLOG_ERROR("Requires protobuf"); \
      return nullptr; \
    } \
    \
    bool ToSVMapEntryData(::ok_msg::SVMapEntryData &entry) override { \
      OKLOG_ERROR("Requires protobuf"); \
      return false; \
    }


  #define OK_SVMAP_PBDYNDATA_SERIALIZABLE() \
    /* =================================================================== */ \
    /* IPBSerializable Members                                             */ \
    \
    inline bool IsIPBSerializable() const noexcept override { return false; } \
    \
    static ::ok::detail::SVEntryWrapperBase::UPtr FromSVMapEntryData( \
        ::ok_msg::SVMapEntryData &entry) { \
      OKLOG_ERROR("Requires protobuf"); \
      return nullptr; \
    } \
    \
    bool ToSVMapEntryData(::ok_msg::SVMapEntryData &entry) override { \
      OKLOG_ERROR("Requires protobuf"); \
      return false; \
    }

  #define OK_SVMAP_IPB_OF_DYNDATA_SERIALIZABLE() \
    /* =================================================================== */ \
    /* IPBSerializable Members                                             */ \
    \
    inline bool IsIPBSerializable() const noexcept override { return false; } \
    \
    static ::ok::detail::SVEntryWrapperBase::UPtr FromSVMapEntryData( \
        ::ok_msg::SVMapEntryData &entry) { \
      OKLOG_ERROR("Requires protobuf"); \
      return nullptr; \
    } \
    \
    bool ToSVMapEntryData(::ok_msg::SVMapEntryData &entry) override { \
      OKLOG_ERROR("Requires protobuf"); \
      return false; \
    }

#endif /* OK_ENABLE_PROTOBUF */



// ===========================================================================
// Core Entry-specific Wrapper-Generating Macro

/**
 * This macro declares a SVEntryWrapperBase subclass that:
 *  * Wraps a specific type of value
 *  * Interops with SVMap & SVStruct to get/set the value
 *  * Handles string-ification of the value for SV{Map,Struct}::ToString()
 *  * Declares a string constant named after the class.  Allows for
 *      SVStruct to generate an entry-unique hash code as well as
 *      for serialization utils to register a name for this class
 *      (i.e. build a string -> classmethod map).
 *  * Optionally includes serialization-related members
 */

#define OK_DECLARE_SV_ENTRY_IMPL( \
    KeyWrapper, \
    ValueT, \
    FunStringify, \
    IPBSerializableMembers) \
  \
  class KeyWrapper final : public ::ok::detail::SVEntryWrapperBase { \
  public: \
    inline KeyWrapper() { } \
    virtual ~KeyWrapper() { } \
    typedef ValueT WrappedType; \
    inline KeyWrapper(KeyWrapper &&k) = default; \
    inline KeyWrapper &operator=(KeyWrapper &&) = default; \
    inline static std::string EntryTypename() noexcept { \
      return std::string(KeyWrapper::Key()); \
    } \
  protected: \
    /* =================================================================== */ \
    /* Utils                                                               */ \
    std::unique_ptr<WrappedType> valp_; \
    friend class ::ok::SVMap; \
    friend class ::ok::SVStruct; \
    friend class ::ok::detail::SVBase; \
    inline static const char* Key() noexcept { \
      /* NB: We must statically declare a var to prevent inlined constants */ \
      static const char* key = OK_XSTRINGIFY(KeyWrapper); \
      return key; \
    } \
    inline const char* ClassKey() noexcept override { \
      return KeyWrapper::Key(); \
    } \
    inline std::string ToString() const override { \
      return FunStringify(GetValue()); \
    } \
    \
    /* =================================================================== */ \
    /* Fulfill the SVMap-friendly wrapper interface                       */ \
    inline explicit KeyWrapper(std::unique_ptr<WrappedType> &&valptr) : \
      valp_(std::move(valptr)) { } \
    inline static ::ok::detail::SVEntryWrapperBase::UPtr WrapPtr( \
                            std::unique_ptr<WrappedType> &&valptr) { \
      return ::ok::detail::SVEntryWrapperBase::UPtr( \
                            new KeyWrapper(std::move(valptr))); \
    } \
    inline \
    static ::ok::detail::SVEntryWrapperBase::UPtr Wrap(WrappedType &&v) { \
      return WrapPtr( \
          std::unique_ptr<WrappedType>(new WrappedType(std::move(v)))); \
    } \
    \
    /* =================================================================== */ \
    /* Get The Value                                                       */ \
    inline WrappedType &GetValueRef() noexcept { return *valp_; } \
    inline const WrappedType &GetValue() const noexcept { return *valp_; } \
    \
    \
    IPBSerializableMembers \
    \
    \
  };

} /* namespace detail */
} /* namespace ok */

#endif /* OK_SV_SVCOMMON_INL_HPP_ */
