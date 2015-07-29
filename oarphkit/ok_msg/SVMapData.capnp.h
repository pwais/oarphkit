// Generated by Cap'n Proto compiler, DO NOT EDIT
// source: SVMapData.capnp

#ifndef CAPNP_INCLUDED_a279f89414de71e4_
#define CAPNP_INCLUDED_a279f89414de71e4_

#include <capnp/generated-header-support.h>

#if CAPNP_VERSION != 6000
#error "Version mismatch between generated code and library headers.  You must use the same version of the Cap'n Proto compiler and library."
#endif


namespace capnp {
namespace schemas {

CAPNP_DECLARE_SCHEMA(b32a60254e87587b);

}  // namespace schemas
}  // namespace capnp

namespace ok_msg {
namespace capnpm {

struct SVMapData {
  SVMapData() = delete;

  class Reader;
  class Builder;
  class Pipeline;

  struct _capnpPrivate {
    CAPNP_DECLARE_STRUCT_HEADER(b32a60254e87587b, 0, 2)
    #if !CAPNP_LITE
    static constexpr ::capnp::_::RawBrandedSchema const* brand = &schema->defaultBrand;
    #endif  // !CAPNP_LITE
  };
};

// =======================================================================================

class SVMapData::Reader {
public:
  typedef SVMapData Reads;

  Reader() = default;
  inline explicit Reader(::capnp::_::StructReader base): _reader(base) {}

  inline ::capnp::MessageSize totalSize() const {
    return _reader.totalSize().asPublic();
  }

#if !CAPNP_LITE
  inline ::kj::StringTree toString() const {
    return ::capnp::_::structString(_reader, *_capnpPrivate::brand);
  }
#endif  // !CAPNP_LITE

  inline bool hasKeys() const;
  inline  ::capnp::List< ::capnp::Text>::Reader getKeys() const;

  inline bool hasValues() const;
  inline  ::capnp::List< ::capnp::Data>::Reader getValues() const;

private:
  ::capnp::_::StructReader _reader;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::ToDynamic_;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::_::PointerHelpers;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::List;
  friend class ::capnp::MessageBuilder;
  friend class ::capnp::Orphanage;
};

class SVMapData::Builder {
public:
  typedef SVMapData Builds;

  Builder() = delete;  // Deleted to discourage incorrect usage.
                       // You can explicitly initialize to nullptr instead.
  inline Builder(decltype(nullptr)) {}
  inline explicit Builder(::capnp::_::StructBuilder base): _builder(base) {}
  inline operator Reader() const { return Reader(_builder.asReader()); }
  inline Reader asReader() const { return *this; }

  inline ::capnp::MessageSize totalSize() const { return asReader().totalSize(); }
#if !CAPNP_LITE
  inline ::kj::StringTree toString() const { return asReader().toString(); }
#endif  // !CAPNP_LITE

  inline bool hasKeys();
  inline  ::capnp::List< ::capnp::Text>::Builder getKeys();
  inline void setKeys( ::capnp::List< ::capnp::Text>::Reader value);
  inline void setKeys(::kj::ArrayPtr<const  ::capnp::Text::Reader> value);
  inline  ::capnp::List< ::capnp::Text>::Builder initKeys(unsigned int size);
  inline void adoptKeys(::capnp::Orphan< ::capnp::List< ::capnp::Text>>&& value);
  inline ::capnp::Orphan< ::capnp::List< ::capnp::Text>> disownKeys();

  inline bool hasValues();
  inline  ::capnp::List< ::capnp::Data>::Builder getValues();
  inline void setValues( ::capnp::List< ::capnp::Data>::Reader value);
  inline void setValues(::kj::ArrayPtr<const  ::capnp::Data::Reader> value);
  inline  ::capnp::List< ::capnp::Data>::Builder initValues(unsigned int size);
  inline void adoptValues(::capnp::Orphan< ::capnp::List< ::capnp::Data>>&& value);
  inline ::capnp::Orphan< ::capnp::List< ::capnp::Data>> disownValues();

private:
  ::capnp::_::StructBuilder _builder;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::ToDynamic_;
  friend class ::capnp::Orphanage;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::_::PointerHelpers;
};

#if !CAPNP_LITE
class SVMapData::Pipeline {
public:
  typedef SVMapData Pipelines;

  inline Pipeline(decltype(nullptr)): _typeless(nullptr) {}
  inline explicit Pipeline(::capnp::AnyPointer::Pipeline&& typeless)
      : _typeless(kj::mv(typeless)) {}

private:
  ::capnp::AnyPointer::Pipeline _typeless;
  friend class ::capnp::PipelineHook;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::ToDynamic_;
};
#endif  // !CAPNP_LITE

// =======================================================================================

inline bool SVMapData::Reader::hasKeys() const {
  return !_reader.getPointerField(0 * ::capnp::POINTERS).isNull();
}
inline bool SVMapData::Builder::hasKeys() {
  return !_builder.getPointerField(0 * ::capnp::POINTERS).isNull();
}
inline  ::capnp::List< ::capnp::Text>::Reader SVMapData::Reader::getKeys() const {
  return ::capnp::_::PointerHelpers< ::capnp::List< ::capnp::Text>>::get(
      _reader.getPointerField(0 * ::capnp::POINTERS));
}
inline  ::capnp::List< ::capnp::Text>::Builder SVMapData::Builder::getKeys() {
  return ::capnp::_::PointerHelpers< ::capnp::List< ::capnp::Text>>::get(
      _builder.getPointerField(0 * ::capnp::POINTERS));
}
inline void SVMapData::Builder::setKeys( ::capnp::List< ::capnp::Text>::Reader value) {
  ::capnp::_::PointerHelpers< ::capnp::List< ::capnp::Text>>::set(
      _builder.getPointerField(0 * ::capnp::POINTERS), value);
}
inline void SVMapData::Builder::setKeys(::kj::ArrayPtr<const  ::capnp::Text::Reader> value) {
  ::capnp::_::PointerHelpers< ::capnp::List< ::capnp::Text>>::set(
      _builder.getPointerField(0 * ::capnp::POINTERS), value);
}
inline  ::capnp::List< ::capnp::Text>::Builder SVMapData::Builder::initKeys(unsigned int size) {
  return ::capnp::_::PointerHelpers< ::capnp::List< ::capnp::Text>>::init(
      _builder.getPointerField(0 * ::capnp::POINTERS), size);
}
inline void SVMapData::Builder::adoptKeys(
    ::capnp::Orphan< ::capnp::List< ::capnp::Text>>&& value) {
  ::capnp::_::PointerHelpers< ::capnp::List< ::capnp::Text>>::adopt(
      _builder.getPointerField(0 * ::capnp::POINTERS), kj::mv(value));
}
inline ::capnp::Orphan< ::capnp::List< ::capnp::Text>> SVMapData::Builder::disownKeys() {
  return ::capnp::_::PointerHelpers< ::capnp::List< ::capnp::Text>>::disown(
      _builder.getPointerField(0 * ::capnp::POINTERS));
}

inline bool SVMapData::Reader::hasValues() const {
  return !_reader.getPointerField(1 * ::capnp::POINTERS).isNull();
}
inline bool SVMapData::Builder::hasValues() {
  return !_builder.getPointerField(1 * ::capnp::POINTERS).isNull();
}
inline  ::capnp::List< ::capnp::Data>::Reader SVMapData::Reader::getValues() const {
  return ::capnp::_::PointerHelpers< ::capnp::List< ::capnp::Data>>::get(
      _reader.getPointerField(1 * ::capnp::POINTERS));
}
inline  ::capnp::List< ::capnp::Data>::Builder SVMapData::Builder::getValues() {
  return ::capnp::_::PointerHelpers< ::capnp::List< ::capnp::Data>>::get(
      _builder.getPointerField(1 * ::capnp::POINTERS));
}
inline void SVMapData::Builder::setValues( ::capnp::List< ::capnp::Data>::Reader value) {
  ::capnp::_::PointerHelpers< ::capnp::List< ::capnp::Data>>::set(
      _builder.getPointerField(1 * ::capnp::POINTERS), value);
}
inline void SVMapData::Builder::setValues(::kj::ArrayPtr<const  ::capnp::Data::Reader> value) {
  ::capnp::_::PointerHelpers< ::capnp::List< ::capnp::Data>>::set(
      _builder.getPointerField(1 * ::capnp::POINTERS), value);
}
inline  ::capnp::List< ::capnp::Data>::Builder SVMapData::Builder::initValues(unsigned int size) {
  return ::capnp::_::PointerHelpers< ::capnp::List< ::capnp::Data>>::init(
      _builder.getPointerField(1 * ::capnp::POINTERS), size);
}
inline void SVMapData::Builder::adoptValues(
    ::capnp::Orphan< ::capnp::List< ::capnp::Data>>&& value) {
  ::capnp::_::PointerHelpers< ::capnp::List< ::capnp::Data>>::adopt(
      _builder.getPointerField(1 * ::capnp::POINTERS), kj::mv(value));
}
inline ::capnp::Orphan< ::capnp::List< ::capnp::Data>> SVMapData::Builder::disownValues() {
  return ::capnp::_::PointerHelpers< ::capnp::List< ::capnp::Data>>::disown(
      _builder.getPointerField(1 * ::capnp::POINTERS));
}

}  // namespace
}  // namespace

#endif  // CAPNP_INCLUDED_a279f89414de71e4_
