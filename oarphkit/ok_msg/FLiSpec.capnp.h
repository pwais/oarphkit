// Generated by Cap'n Proto compiler, DO NOT EDIT
// source: FLiSpec.capnp

#ifndef CAPNP_INCLUDED_bea3fb14aa6f9bdb_
#define CAPNP_INCLUDED_bea3fb14aa6f9bdb_

#include <capnp/generated-header-support.h>

#if CAPNP_VERSION != 6000
#error "Version mismatch between generated code and library headers.  You must use the same version of the Cap'n Proto compiler and library."
#endif


namespace capnp {
namespace schemas {

CAPNP_DECLARE_SCHEMA(da358bcebb4802c0);
CAPNP_DECLARE_SCHEMA(fd7f1091ab4ce2b8);
CAPNP_DECLARE_SCHEMA(e9408d9abe22096c);
CAPNP_DECLARE_SCHEMA(b8c42a40a0f4d775);
CAPNP_DECLARE_SCHEMA(807c7c6ebd0a3460);

}  // namespace schemas
}  // namespace capnp

namespace ok_msg {
namespace capnpm {

struct FuncInitDatum {
  FuncInitDatum() = delete;

  class Reader;
  class Builder;
  class Pipeline;
  struct KV;

  struct _capnpPrivate {
    CAPNP_DECLARE_STRUCT_HEADER(da358bcebb4802c0, 0, 3)
    #if !CAPNP_LITE
    static constexpr ::capnp::_::RawBrandedSchema const* brand = &schema->defaultBrand;
    #endif  // !CAPNP_LITE
  };
};

struct FuncInitDatum::KV {
  KV() = delete;

  class Reader;
  class Builder;
  class Pipeline;

  struct _capnpPrivate {
    CAPNP_DECLARE_STRUCT_HEADER(fd7f1091ab4ce2b8, 0, 2)
    #if !CAPNP_LITE
    static constexpr ::capnp::_::RawBrandedSchema const* brand = &schema->defaultBrand;
    #endif  // !CAPNP_LITE
  };
};

struct FunctorNode {
  FunctorNode() = delete;

  class Reader;
  class Builder;
  class Pipeline;
  struct Func;
  struct Composition;

  struct _capnpPrivate {
    CAPNP_DECLARE_STRUCT_HEADER(e9408d9abe22096c, 0, 0)
    #if !CAPNP_LITE
    static constexpr ::capnp::_::RawBrandedSchema const* brand = &schema->defaultBrand;
    #endif  // !CAPNP_LITE
  };
};

struct FunctorNode::Func {
  Func() = delete;

  class Reader;
  class Builder;
  class Pipeline;

  struct _capnpPrivate {
    CAPNP_DECLARE_STRUCT_HEADER(b8c42a40a0f4d775, 0, 3)
    #if !CAPNP_LITE
    static constexpr ::capnp::_::RawBrandedSchema const* brand = &schema->defaultBrand;
    #endif  // !CAPNP_LITE
  };
};

struct FunctorNode::Composition {
  Composition() = delete;

  class Reader;
  class Builder;
  class Pipeline;

  struct _capnpPrivate {
    CAPNP_DECLARE_STRUCT_HEADER(807c7c6ebd0a3460, 0, 2)
    #if !CAPNP_LITE
    static constexpr ::capnp::_::RawBrandedSchema const* brand = &schema->defaultBrand;
    #endif  // !CAPNP_LITE
  };
};

// =======================================================================================

class FuncInitDatum::Reader {
public:
  typedef FuncInitDatum Reads;

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

  inline bool hasRawBytes() const;
  inline  ::capnp::Data::Reader getRawBytes() const;

  inline bool hasRawStr() const;
  inline  ::capnp::Text::Reader getRawStr() const;

  inline bool hasKvs() const;
  inline  ::capnp::List< ::ok_msg::capnpm::FuncInitDatum::KV>::Reader getKvs() const;

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

class FuncInitDatum::Builder {
public:
  typedef FuncInitDatum Builds;

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

  inline bool hasRawBytes();
  inline  ::capnp::Data::Builder getRawBytes();
  inline void setRawBytes( ::capnp::Data::Reader value);
  inline  ::capnp::Data::Builder initRawBytes(unsigned int size);
  inline void adoptRawBytes(::capnp::Orphan< ::capnp::Data>&& value);
  inline ::capnp::Orphan< ::capnp::Data> disownRawBytes();

  inline bool hasRawStr();
  inline  ::capnp::Text::Builder getRawStr();
  inline void setRawStr( ::capnp::Text::Reader value);
  inline  ::capnp::Text::Builder initRawStr(unsigned int size);
  inline void adoptRawStr(::capnp::Orphan< ::capnp::Text>&& value);
  inline ::capnp::Orphan< ::capnp::Text> disownRawStr();

  inline bool hasKvs();
  inline  ::capnp::List< ::ok_msg::capnpm::FuncInitDatum::KV>::Builder getKvs();
  inline void setKvs( ::capnp::List< ::ok_msg::capnpm::FuncInitDatum::KV>::Reader value);
  inline  ::capnp::List< ::ok_msg::capnpm::FuncInitDatum::KV>::Builder initKvs(unsigned int size);
  inline void adoptKvs(::capnp::Orphan< ::capnp::List< ::ok_msg::capnpm::FuncInitDatum::KV>>&& value);
  inline ::capnp::Orphan< ::capnp::List< ::ok_msg::capnpm::FuncInitDatum::KV>> disownKvs();

private:
  ::capnp::_::StructBuilder _builder;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::ToDynamic_;
  friend class ::capnp::Orphanage;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::_::PointerHelpers;
};

#if !CAPNP_LITE
class FuncInitDatum::Pipeline {
public:
  typedef FuncInitDatum Pipelines;

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

class FuncInitDatum::KV::Reader {
public:
  typedef KV Reads;

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

  inline bool hasKey() const;
  inline  ::capnp::Text::Reader getKey() const;

  inline bool hasValue() const;
  inline  ::capnp::Text::Reader getValue() const;

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

class FuncInitDatum::KV::Builder {
public:
  typedef KV Builds;

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

  inline bool hasKey();
  inline  ::capnp::Text::Builder getKey();
  inline void setKey( ::capnp::Text::Reader value);
  inline  ::capnp::Text::Builder initKey(unsigned int size);
  inline void adoptKey(::capnp::Orphan< ::capnp::Text>&& value);
  inline ::capnp::Orphan< ::capnp::Text> disownKey();

  inline bool hasValue();
  inline  ::capnp::Text::Builder getValue();
  inline void setValue( ::capnp::Text::Reader value);
  inline  ::capnp::Text::Builder initValue(unsigned int size);
  inline void adoptValue(::capnp::Orphan< ::capnp::Text>&& value);
  inline ::capnp::Orphan< ::capnp::Text> disownValue();

private:
  ::capnp::_::StructBuilder _builder;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::ToDynamic_;
  friend class ::capnp::Orphanage;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::_::PointerHelpers;
};

#if !CAPNP_LITE
class FuncInitDatum::KV::Pipeline {
public:
  typedef KV Pipelines;

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

class FunctorNode::Reader {
public:
  typedef FunctorNode Reads;

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

class FunctorNode::Builder {
public:
  typedef FunctorNode Builds;

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

private:
  ::capnp::_::StructBuilder _builder;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::ToDynamic_;
  friend class ::capnp::Orphanage;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::_::PointerHelpers;
};

#if !CAPNP_LITE
class FunctorNode::Pipeline {
public:
  typedef FunctorNode Pipelines;

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

class FunctorNode::Func::Reader {
public:
  typedef Func Reads;

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

  inline bool hasAlias() const;
  inline  ::capnp::Text::Reader getAlias() const;

  inline bool hasInit() const;
  inline  ::ok_msg::capnpm::FuncInitDatum::Reader getInit() const;

  inline bool hasName() const;
  inline  ::capnp::Text::Reader getName() const;

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

class FunctorNode::Func::Builder {
public:
  typedef Func Builds;

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

  inline bool hasAlias();
  inline  ::capnp::Text::Builder getAlias();
  inline void setAlias( ::capnp::Text::Reader value);
  inline  ::capnp::Text::Builder initAlias(unsigned int size);
  inline void adoptAlias(::capnp::Orphan< ::capnp::Text>&& value);
  inline ::capnp::Orphan< ::capnp::Text> disownAlias();

  inline bool hasInit();
  inline  ::ok_msg::capnpm::FuncInitDatum::Builder getInit();
  inline void setInit( ::ok_msg::capnpm::FuncInitDatum::Reader value);
  inline  ::ok_msg::capnpm::FuncInitDatum::Builder initInit();
  inline void adoptInit(::capnp::Orphan< ::ok_msg::capnpm::FuncInitDatum>&& value);
  inline ::capnp::Orphan< ::ok_msg::capnpm::FuncInitDatum> disownInit();

  inline bool hasName();
  inline  ::capnp::Text::Builder getName();
  inline void setName( ::capnp::Text::Reader value);
  inline  ::capnp::Text::Builder initName(unsigned int size);
  inline void adoptName(::capnp::Orphan< ::capnp::Text>&& value);
  inline ::capnp::Orphan< ::capnp::Text> disownName();

private:
  ::capnp::_::StructBuilder _builder;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::ToDynamic_;
  friend class ::capnp::Orphanage;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::_::PointerHelpers;
};

#if !CAPNP_LITE
class FunctorNode::Func::Pipeline {
public:
  typedef Func Pipelines;

  inline Pipeline(decltype(nullptr)): _typeless(nullptr) {}
  inline explicit Pipeline(::capnp::AnyPointer::Pipeline&& typeless)
      : _typeless(kj::mv(typeless)) {}

  inline  ::ok_msg::capnpm::FuncInitDatum::Pipeline getInit();
private:
  ::capnp::AnyPointer::Pipeline _typeless;
  friend class ::capnp::PipelineHook;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::ToDynamic_;
};
#endif  // !CAPNP_LITE

class FunctorNode::Composition::Reader {
public:
  typedef Composition Reads;

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

  inline bool hasFuncs() const;
  inline  ::capnp::List< ::ok_msg::capnpm::FunctorNode::Func>::Reader getFuncs() const;

  inline bool hasInit() const;
  inline  ::ok_msg::capnpm::FuncInitDatum::Reader getInit() const;

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

class FunctorNode::Composition::Builder {
public:
  typedef Composition Builds;

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

  inline bool hasFuncs();
  inline  ::capnp::List< ::ok_msg::capnpm::FunctorNode::Func>::Builder getFuncs();
  inline void setFuncs( ::capnp::List< ::ok_msg::capnpm::FunctorNode::Func>::Reader value);
  inline  ::capnp::List< ::ok_msg::capnpm::FunctorNode::Func>::Builder initFuncs(unsigned int size);
  inline void adoptFuncs(::capnp::Orphan< ::capnp::List< ::ok_msg::capnpm::FunctorNode::Func>>&& value);
  inline ::capnp::Orphan< ::capnp::List< ::ok_msg::capnpm::FunctorNode::Func>> disownFuncs();

  inline bool hasInit();
  inline  ::ok_msg::capnpm::FuncInitDatum::Builder getInit();
  inline void setInit( ::ok_msg::capnpm::FuncInitDatum::Reader value);
  inline  ::ok_msg::capnpm::FuncInitDatum::Builder initInit();
  inline void adoptInit(::capnp::Orphan< ::ok_msg::capnpm::FuncInitDatum>&& value);
  inline ::capnp::Orphan< ::ok_msg::capnpm::FuncInitDatum> disownInit();

private:
  ::capnp::_::StructBuilder _builder;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::ToDynamic_;
  friend class ::capnp::Orphanage;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::_::PointerHelpers;
};

#if !CAPNP_LITE
class FunctorNode::Composition::Pipeline {
public:
  typedef Composition Pipelines;

  inline Pipeline(decltype(nullptr)): _typeless(nullptr) {}
  inline explicit Pipeline(::capnp::AnyPointer::Pipeline&& typeless)
      : _typeless(kj::mv(typeless)) {}

  inline  ::ok_msg::capnpm::FuncInitDatum::Pipeline getInit();
private:
  ::capnp::AnyPointer::Pipeline _typeless;
  friend class ::capnp::PipelineHook;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::ToDynamic_;
};
#endif  // !CAPNP_LITE

// =======================================================================================

inline bool FuncInitDatum::Reader::hasRawBytes() const {
  return !_reader.getPointerField(0 * ::capnp::POINTERS).isNull();
}
inline bool FuncInitDatum::Builder::hasRawBytes() {
  return !_builder.getPointerField(0 * ::capnp::POINTERS).isNull();
}
inline  ::capnp::Data::Reader FuncInitDatum::Reader::getRawBytes() const {
  return ::capnp::_::PointerHelpers< ::capnp::Data>::get(
      _reader.getPointerField(0 * ::capnp::POINTERS));
}
inline  ::capnp::Data::Builder FuncInitDatum::Builder::getRawBytes() {
  return ::capnp::_::PointerHelpers< ::capnp::Data>::get(
      _builder.getPointerField(0 * ::capnp::POINTERS));
}
inline void FuncInitDatum::Builder::setRawBytes( ::capnp::Data::Reader value) {
  ::capnp::_::PointerHelpers< ::capnp::Data>::set(
      _builder.getPointerField(0 * ::capnp::POINTERS), value);
}
inline  ::capnp::Data::Builder FuncInitDatum::Builder::initRawBytes(unsigned int size) {
  return ::capnp::_::PointerHelpers< ::capnp::Data>::init(
      _builder.getPointerField(0 * ::capnp::POINTERS), size);
}
inline void FuncInitDatum::Builder::adoptRawBytes(
    ::capnp::Orphan< ::capnp::Data>&& value) {
  ::capnp::_::PointerHelpers< ::capnp::Data>::adopt(
      _builder.getPointerField(0 * ::capnp::POINTERS), kj::mv(value));
}
inline ::capnp::Orphan< ::capnp::Data> FuncInitDatum::Builder::disownRawBytes() {
  return ::capnp::_::PointerHelpers< ::capnp::Data>::disown(
      _builder.getPointerField(0 * ::capnp::POINTERS));
}

inline bool FuncInitDatum::Reader::hasRawStr() const {
  return !_reader.getPointerField(1 * ::capnp::POINTERS).isNull();
}
inline bool FuncInitDatum::Builder::hasRawStr() {
  return !_builder.getPointerField(1 * ::capnp::POINTERS).isNull();
}
inline  ::capnp::Text::Reader FuncInitDatum::Reader::getRawStr() const {
  return ::capnp::_::PointerHelpers< ::capnp::Text>::get(
      _reader.getPointerField(1 * ::capnp::POINTERS));
}
inline  ::capnp::Text::Builder FuncInitDatum::Builder::getRawStr() {
  return ::capnp::_::PointerHelpers< ::capnp::Text>::get(
      _builder.getPointerField(1 * ::capnp::POINTERS));
}
inline void FuncInitDatum::Builder::setRawStr( ::capnp::Text::Reader value) {
  ::capnp::_::PointerHelpers< ::capnp::Text>::set(
      _builder.getPointerField(1 * ::capnp::POINTERS), value);
}
inline  ::capnp::Text::Builder FuncInitDatum::Builder::initRawStr(unsigned int size) {
  return ::capnp::_::PointerHelpers< ::capnp::Text>::init(
      _builder.getPointerField(1 * ::capnp::POINTERS), size);
}
inline void FuncInitDatum::Builder::adoptRawStr(
    ::capnp::Orphan< ::capnp::Text>&& value) {
  ::capnp::_::PointerHelpers< ::capnp::Text>::adopt(
      _builder.getPointerField(1 * ::capnp::POINTERS), kj::mv(value));
}
inline ::capnp::Orphan< ::capnp::Text> FuncInitDatum::Builder::disownRawStr() {
  return ::capnp::_::PointerHelpers< ::capnp::Text>::disown(
      _builder.getPointerField(1 * ::capnp::POINTERS));
}

inline bool FuncInitDatum::Reader::hasKvs() const {
  return !_reader.getPointerField(2 * ::capnp::POINTERS).isNull();
}
inline bool FuncInitDatum::Builder::hasKvs() {
  return !_builder.getPointerField(2 * ::capnp::POINTERS).isNull();
}
inline  ::capnp::List< ::ok_msg::capnpm::FuncInitDatum::KV>::Reader FuncInitDatum::Reader::getKvs() const {
  return ::capnp::_::PointerHelpers< ::capnp::List< ::ok_msg::capnpm::FuncInitDatum::KV>>::get(
      _reader.getPointerField(2 * ::capnp::POINTERS));
}
inline  ::capnp::List< ::ok_msg::capnpm::FuncInitDatum::KV>::Builder FuncInitDatum::Builder::getKvs() {
  return ::capnp::_::PointerHelpers< ::capnp::List< ::ok_msg::capnpm::FuncInitDatum::KV>>::get(
      _builder.getPointerField(2 * ::capnp::POINTERS));
}
inline void FuncInitDatum::Builder::setKvs( ::capnp::List< ::ok_msg::capnpm::FuncInitDatum::KV>::Reader value) {
  ::capnp::_::PointerHelpers< ::capnp::List< ::ok_msg::capnpm::FuncInitDatum::KV>>::set(
      _builder.getPointerField(2 * ::capnp::POINTERS), value);
}
inline  ::capnp::List< ::ok_msg::capnpm::FuncInitDatum::KV>::Builder FuncInitDatum::Builder::initKvs(unsigned int size) {
  return ::capnp::_::PointerHelpers< ::capnp::List< ::ok_msg::capnpm::FuncInitDatum::KV>>::init(
      _builder.getPointerField(2 * ::capnp::POINTERS), size);
}
inline void FuncInitDatum::Builder::adoptKvs(
    ::capnp::Orphan< ::capnp::List< ::ok_msg::capnpm::FuncInitDatum::KV>>&& value) {
  ::capnp::_::PointerHelpers< ::capnp::List< ::ok_msg::capnpm::FuncInitDatum::KV>>::adopt(
      _builder.getPointerField(2 * ::capnp::POINTERS), kj::mv(value));
}
inline ::capnp::Orphan< ::capnp::List< ::ok_msg::capnpm::FuncInitDatum::KV>> FuncInitDatum::Builder::disownKvs() {
  return ::capnp::_::PointerHelpers< ::capnp::List< ::ok_msg::capnpm::FuncInitDatum::KV>>::disown(
      _builder.getPointerField(2 * ::capnp::POINTERS));
}

inline bool FuncInitDatum::KV::Reader::hasKey() const {
  return !_reader.getPointerField(0 * ::capnp::POINTERS).isNull();
}
inline bool FuncInitDatum::KV::Builder::hasKey() {
  return !_builder.getPointerField(0 * ::capnp::POINTERS).isNull();
}
inline  ::capnp::Text::Reader FuncInitDatum::KV::Reader::getKey() const {
  return ::capnp::_::PointerHelpers< ::capnp::Text>::get(
      _reader.getPointerField(0 * ::capnp::POINTERS));
}
inline  ::capnp::Text::Builder FuncInitDatum::KV::Builder::getKey() {
  return ::capnp::_::PointerHelpers< ::capnp::Text>::get(
      _builder.getPointerField(0 * ::capnp::POINTERS));
}
inline void FuncInitDatum::KV::Builder::setKey( ::capnp::Text::Reader value) {
  ::capnp::_::PointerHelpers< ::capnp::Text>::set(
      _builder.getPointerField(0 * ::capnp::POINTERS), value);
}
inline  ::capnp::Text::Builder FuncInitDatum::KV::Builder::initKey(unsigned int size) {
  return ::capnp::_::PointerHelpers< ::capnp::Text>::init(
      _builder.getPointerField(0 * ::capnp::POINTERS), size);
}
inline void FuncInitDatum::KV::Builder::adoptKey(
    ::capnp::Orphan< ::capnp::Text>&& value) {
  ::capnp::_::PointerHelpers< ::capnp::Text>::adopt(
      _builder.getPointerField(0 * ::capnp::POINTERS), kj::mv(value));
}
inline ::capnp::Orphan< ::capnp::Text> FuncInitDatum::KV::Builder::disownKey() {
  return ::capnp::_::PointerHelpers< ::capnp::Text>::disown(
      _builder.getPointerField(0 * ::capnp::POINTERS));
}

inline bool FuncInitDatum::KV::Reader::hasValue() const {
  return !_reader.getPointerField(1 * ::capnp::POINTERS).isNull();
}
inline bool FuncInitDatum::KV::Builder::hasValue() {
  return !_builder.getPointerField(1 * ::capnp::POINTERS).isNull();
}
inline  ::capnp::Text::Reader FuncInitDatum::KV::Reader::getValue() const {
  return ::capnp::_::PointerHelpers< ::capnp::Text>::get(
      _reader.getPointerField(1 * ::capnp::POINTERS));
}
inline  ::capnp::Text::Builder FuncInitDatum::KV::Builder::getValue() {
  return ::capnp::_::PointerHelpers< ::capnp::Text>::get(
      _builder.getPointerField(1 * ::capnp::POINTERS));
}
inline void FuncInitDatum::KV::Builder::setValue( ::capnp::Text::Reader value) {
  ::capnp::_::PointerHelpers< ::capnp::Text>::set(
      _builder.getPointerField(1 * ::capnp::POINTERS), value);
}
inline  ::capnp::Text::Builder FuncInitDatum::KV::Builder::initValue(unsigned int size) {
  return ::capnp::_::PointerHelpers< ::capnp::Text>::init(
      _builder.getPointerField(1 * ::capnp::POINTERS), size);
}
inline void FuncInitDatum::KV::Builder::adoptValue(
    ::capnp::Orphan< ::capnp::Text>&& value) {
  ::capnp::_::PointerHelpers< ::capnp::Text>::adopt(
      _builder.getPointerField(1 * ::capnp::POINTERS), kj::mv(value));
}
inline ::capnp::Orphan< ::capnp::Text> FuncInitDatum::KV::Builder::disownValue() {
  return ::capnp::_::PointerHelpers< ::capnp::Text>::disown(
      _builder.getPointerField(1 * ::capnp::POINTERS));
}

inline bool FunctorNode::Func::Reader::hasAlias() const {
  return !_reader.getPointerField(0 * ::capnp::POINTERS).isNull();
}
inline bool FunctorNode::Func::Builder::hasAlias() {
  return !_builder.getPointerField(0 * ::capnp::POINTERS).isNull();
}
inline  ::capnp::Text::Reader FunctorNode::Func::Reader::getAlias() const {
  return ::capnp::_::PointerHelpers< ::capnp::Text>::get(
      _reader.getPointerField(0 * ::capnp::POINTERS));
}
inline  ::capnp::Text::Builder FunctorNode::Func::Builder::getAlias() {
  return ::capnp::_::PointerHelpers< ::capnp::Text>::get(
      _builder.getPointerField(0 * ::capnp::POINTERS));
}
inline void FunctorNode::Func::Builder::setAlias( ::capnp::Text::Reader value) {
  ::capnp::_::PointerHelpers< ::capnp::Text>::set(
      _builder.getPointerField(0 * ::capnp::POINTERS), value);
}
inline  ::capnp::Text::Builder FunctorNode::Func::Builder::initAlias(unsigned int size) {
  return ::capnp::_::PointerHelpers< ::capnp::Text>::init(
      _builder.getPointerField(0 * ::capnp::POINTERS), size);
}
inline void FunctorNode::Func::Builder::adoptAlias(
    ::capnp::Orphan< ::capnp::Text>&& value) {
  ::capnp::_::PointerHelpers< ::capnp::Text>::adopt(
      _builder.getPointerField(0 * ::capnp::POINTERS), kj::mv(value));
}
inline ::capnp::Orphan< ::capnp::Text> FunctorNode::Func::Builder::disownAlias() {
  return ::capnp::_::PointerHelpers< ::capnp::Text>::disown(
      _builder.getPointerField(0 * ::capnp::POINTERS));
}

inline bool FunctorNode::Func::Reader::hasInit() const {
  return !_reader.getPointerField(1 * ::capnp::POINTERS).isNull();
}
inline bool FunctorNode::Func::Builder::hasInit() {
  return !_builder.getPointerField(1 * ::capnp::POINTERS).isNull();
}
inline  ::ok_msg::capnpm::FuncInitDatum::Reader FunctorNode::Func::Reader::getInit() const {
  return ::capnp::_::PointerHelpers< ::ok_msg::capnpm::FuncInitDatum>::get(
      _reader.getPointerField(1 * ::capnp::POINTERS));
}
inline  ::ok_msg::capnpm::FuncInitDatum::Builder FunctorNode::Func::Builder::getInit() {
  return ::capnp::_::PointerHelpers< ::ok_msg::capnpm::FuncInitDatum>::get(
      _builder.getPointerField(1 * ::capnp::POINTERS));
}
#if !CAPNP_LITE
inline  ::ok_msg::capnpm::FuncInitDatum::Pipeline FunctorNode::Func::Pipeline::getInit() {
  return  ::ok_msg::capnpm::FuncInitDatum::Pipeline(_typeless.getPointerField(1));
}
#endif  // !CAPNP_LITE
inline void FunctorNode::Func::Builder::setInit( ::ok_msg::capnpm::FuncInitDatum::Reader value) {
  ::capnp::_::PointerHelpers< ::ok_msg::capnpm::FuncInitDatum>::set(
      _builder.getPointerField(1 * ::capnp::POINTERS), value);
}
inline  ::ok_msg::capnpm::FuncInitDatum::Builder FunctorNode::Func::Builder::initInit() {
  return ::capnp::_::PointerHelpers< ::ok_msg::capnpm::FuncInitDatum>::init(
      _builder.getPointerField(1 * ::capnp::POINTERS));
}
inline void FunctorNode::Func::Builder::adoptInit(
    ::capnp::Orphan< ::ok_msg::capnpm::FuncInitDatum>&& value) {
  ::capnp::_::PointerHelpers< ::ok_msg::capnpm::FuncInitDatum>::adopt(
      _builder.getPointerField(1 * ::capnp::POINTERS), kj::mv(value));
}
inline ::capnp::Orphan< ::ok_msg::capnpm::FuncInitDatum> FunctorNode::Func::Builder::disownInit() {
  return ::capnp::_::PointerHelpers< ::ok_msg::capnpm::FuncInitDatum>::disown(
      _builder.getPointerField(1 * ::capnp::POINTERS));
}

inline bool FunctorNode::Func::Reader::hasName() const {
  return !_reader.getPointerField(2 * ::capnp::POINTERS).isNull();
}
inline bool FunctorNode::Func::Builder::hasName() {
  return !_builder.getPointerField(2 * ::capnp::POINTERS).isNull();
}
inline  ::capnp::Text::Reader FunctorNode::Func::Reader::getName() const {
  return ::capnp::_::PointerHelpers< ::capnp::Text>::get(
      _reader.getPointerField(2 * ::capnp::POINTERS));
}
inline  ::capnp::Text::Builder FunctorNode::Func::Builder::getName() {
  return ::capnp::_::PointerHelpers< ::capnp::Text>::get(
      _builder.getPointerField(2 * ::capnp::POINTERS));
}
inline void FunctorNode::Func::Builder::setName( ::capnp::Text::Reader value) {
  ::capnp::_::PointerHelpers< ::capnp::Text>::set(
      _builder.getPointerField(2 * ::capnp::POINTERS), value);
}
inline  ::capnp::Text::Builder FunctorNode::Func::Builder::initName(unsigned int size) {
  return ::capnp::_::PointerHelpers< ::capnp::Text>::init(
      _builder.getPointerField(2 * ::capnp::POINTERS), size);
}
inline void FunctorNode::Func::Builder::adoptName(
    ::capnp::Orphan< ::capnp::Text>&& value) {
  ::capnp::_::PointerHelpers< ::capnp::Text>::adopt(
      _builder.getPointerField(2 * ::capnp::POINTERS), kj::mv(value));
}
inline ::capnp::Orphan< ::capnp::Text> FunctorNode::Func::Builder::disownName() {
  return ::capnp::_::PointerHelpers< ::capnp::Text>::disown(
      _builder.getPointerField(2 * ::capnp::POINTERS));
}

inline bool FunctorNode::Composition::Reader::hasFuncs() const {
  return !_reader.getPointerField(0 * ::capnp::POINTERS).isNull();
}
inline bool FunctorNode::Composition::Builder::hasFuncs() {
  return !_builder.getPointerField(0 * ::capnp::POINTERS).isNull();
}
inline  ::capnp::List< ::ok_msg::capnpm::FunctorNode::Func>::Reader FunctorNode::Composition::Reader::getFuncs() const {
  return ::capnp::_::PointerHelpers< ::capnp::List< ::ok_msg::capnpm::FunctorNode::Func>>::get(
      _reader.getPointerField(0 * ::capnp::POINTERS));
}
inline  ::capnp::List< ::ok_msg::capnpm::FunctorNode::Func>::Builder FunctorNode::Composition::Builder::getFuncs() {
  return ::capnp::_::PointerHelpers< ::capnp::List< ::ok_msg::capnpm::FunctorNode::Func>>::get(
      _builder.getPointerField(0 * ::capnp::POINTERS));
}
inline void FunctorNode::Composition::Builder::setFuncs( ::capnp::List< ::ok_msg::capnpm::FunctorNode::Func>::Reader value) {
  ::capnp::_::PointerHelpers< ::capnp::List< ::ok_msg::capnpm::FunctorNode::Func>>::set(
      _builder.getPointerField(0 * ::capnp::POINTERS), value);
}
inline  ::capnp::List< ::ok_msg::capnpm::FunctorNode::Func>::Builder FunctorNode::Composition::Builder::initFuncs(unsigned int size) {
  return ::capnp::_::PointerHelpers< ::capnp::List< ::ok_msg::capnpm::FunctorNode::Func>>::init(
      _builder.getPointerField(0 * ::capnp::POINTERS), size);
}
inline void FunctorNode::Composition::Builder::adoptFuncs(
    ::capnp::Orphan< ::capnp::List< ::ok_msg::capnpm::FunctorNode::Func>>&& value) {
  ::capnp::_::PointerHelpers< ::capnp::List< ::ok_msg::capnpm::FunctorNode::Func>>::adopt(
      _builder.getPointerField(0 * ::capnp::POINTERS), kj::mv(value));
}
inline ::capnp::Orphan< ::capnp::List< ::ok_msg::capnpm::FunctorNode::Func>> FunctorNode::Composition::Builder::disownFuncs() {
  return ::capnp::_::PointerHelpers< ::capnp::List< ::ok_msg::capnpm::FunctorNode::Func>>::disown(
      _builder.getPointerField(0 * ::capnp::POINTERS));
}

inline bool FunctorNode::Composition::Reader::hasInit() const {
  return !_reader.getPointerField(1 * ::capnp::POINTERS).isNull();
}
inline bool FunctorNode::Composition::Builder::hasInit() {
  return !_builder.getPointerField(1 * ::capnp::POINTERS).isNull();
}
inline  ::ok_msg::capnpm::FuncInitDatum::Reader FunctorNode::Composition::Reader::getInit() const {
  return ::capnp::_::PointerHelpers< ::ok_msg::capnpm::FuncInitDatum>::get(
      _reader.getPointerField(1 * ::capnp::POINTERS));
}
inline  ::ok_msg::capnpm::FuncInitDatum::Builder FunctorNode::Composition::Builder::getInit() {
  return ::capnp::_::PointerHelpers< ::ok_msg::capnpm::FuncInitDatum>::get(
      _builder.getPointerField(1 * ::capnp::POINTERS));
}
#if !CAPNP_LITE
inline  ::ok_msg::capnpm::FuncInitDatum::Pipeline FunctorNode::Composition::Pipeline::getInit() {
  return  ::ok_msg::capnpm::FuncInitDatum::Pipeline(_typeless.getPointerField(1));
}
#endif  // !CAPNP_LITE
inline void FunctorNode::Composition::Builder::setInit( ::ok_msg::capnpm::FuncInitDatum::Reader value) {
  ::capnp::_::PointerHelpers< ::ok_msg::capnpm::FuncInitDatum>::set(
      _builder.getPointerField(1 * ::capnp::POINTERS), value);
}
inline  ::ok_msg::capnpm::FuncInitDatum::Builder FunctorNode::Composition::Builder::initInit() {
  return ::capnp::_::PointerHelpers< ::ok_msg::capnpm::FuncInitDatum>::init(
      _builder.getPointerField(1 * ::capnp::POINTERS));
}
inline void FunctorNode::Composition::Builder::adoptInit(
    ::capnp::Orphan< ::ok_msg::capnpm::FuncInitDatum>&& value) {
  ::capnp::_::PointerHelpers< ::ok_msg::capnpm::FuncInitDatum>::adopt(
      _builder.getPointerField(1 * ::capnp::POINTERS), kj::mv(value));
}
inline ::capnp::Orphan< ::ok_msg::capnpm::FuncInitDatum> FunctorNode::Composition::Builder::disownInit() {
  return ::capnp::_::PointerHelpers< ::ok_msg::capnpm::FuncInitDatum>::disown(
      _builder.getPointerField(1 * ::capnp::POINTERS));
}

}  // namespace
}  // namespace

#endif  // CAPNP_INCLUDED_bea3fb14aa6f9bdb_
