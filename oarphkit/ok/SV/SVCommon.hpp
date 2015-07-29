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

#ifndef OK_SV_SVCOMMON_HPP_
#define OK_SV_SVCOMMON_HPP_

#include "ok/Stringify.hpp"
#include "ok/SV/SVCommon-inl.hpp"

// ===========================================================================
// Declare SVStruct or SVMap Entries

// A base entry (with an unserializable, unstringifiable value)
#define OK_DECLARE_SV_ENTRY(EntryName, ValueType) \
  OK_DECLARE_SV_ENTRY_IMPL( \
    EntryName, \
    ValueType, \
    ::ok::detail::NonStringifiable<ValueType>, \
    OK_SVMAP_NOT_IPBSERIALIZABLE())

// An entry whose value has an operator<<() defined
// (or is otherwise ok::Stringify()able)
#define OK_DECLARE_SV_DSTR_ENTRY(EntryName, ValueType) \
  OK_DECLARE_SV_ENTRY_IMPL( \
    EntryName, \
    ValueType, \
    ::ok::Stringify, \
    OK_SVMAP_NOT_IPBSERIALIZABLE())

// Use ok::Stringify() but with not length limit *peligro*
#define OK_DECLARE_SV_DSTRUL_ENTRY(EntryName, ValueType) \
  OK_DECLARE_SV_ENTRY_IMPL( \
    EntryName, \
    ValueType, \
    ::ok::StringifyNoLimit, \
    OK_SVMAP_NOT_IPBSERIALIZABLE())

// An entry with a user stringification method defined
#define OK_DECLARE_SV_USTR_ENTRY(EntryName, ValueType, FunStringify) \
  OK_DECLARE_SV_ENTRY_IMPL( \
    EntryName, \
    ValueType, \
    FunStringify, \
    OK_SVMAP_NOT_IPBSERIALIZABLE())

#if OK_ENABLE_PROTOBUF

// An IPBSerializable entry with ok::Stringify() and no length limit
#define OK_DECLARE_SV_DSTRUL_IPB_ENTRY(EntryName, ValueType, ExtensionName) \
  OK_DECLARE_SV_ENTRY_IMPL( \
    EntryName, \
    ValueType, \
    ::ok::StringifyNoLimit, \
    OK_SVMAP_IPBSERIALIZABLE(ExtensionName))

// An entry that can be serialized to DynData
// with ok::Stringify() and no length limit
#define OK_DECLARE_SV_DSTRUL_PBDYNDATA_ENTRY(EntryName, ValueType) \
  OK_DECLARE_SV_ENTRY_IMPL( \
    EntryName, \
    ValueType, \
    ::ok::StringifyNoLimit, \
    OK_SVMAP_PBDYNDATA_SERIALIZABLE())

// An entry that can be serialized to DynData
// with ok::Stringify() and standard length limit
#define OK_DECLARE_SV_DSTR_PBDYNDATA_ENTRY(EntryName, ValueType) \
  OK_DECLARE_SV_ENTRY_IMPL( \
    EntryName, \
    ValueType, \
    ::ok::Stringify, \
    OK_SVMAP_PBDYNDATA_SERIALIZABLE())

// An entry that can be serialized to DynData (array)
// with ok::Stringify() and no length limit
#define OK_DECLARE_SV_DSTRUL_PBDYNARR_ENTRY(EntryName, ValueType) \
  OK_DECLARE_SV_ENTRY_IMPL( \
    EntryName, \
    ValueType, \
    ::ok::StringifyNoLimit, \
    OK_SVMAP_PBDYNARR_SERIALIZABLE())

// An entry that can be serialized to DynData (array)
// with ok::Stringify() and standard length limit
#define OK_DECLARE_SV_DSTR_PBDYNARR_ENTRY(EntryName, ValueType) \
  OK_DECLARE_SV_ENTRY_IMPL( \
    EntryName, \
    ValueType, \
    ::ok::Stringify, \
    OK_SVMAP_PBDYNARR_SERIALIZABLE())

// An IPBSerializable<DynamicProto> entry (that can be serialized to
// SVMap's DynData).  Using ok::Stringify() and no length limit
#define OK_DECLARE_SV_DSTRUL_IPB_OF_DYNDATA_ENTRY(EntryName, ValueType) \
  OK_DECLARE_SV_ENTRY_IMPL( \
    EntryName, \
    ValueType, \
    ::ok::StringifyNoLimit, \
    OK_SVMAP_IPB_OF_DYNDATA_SERIALIZABLE())

#else /* no protobuf */

// An IPBSerializable entry with ok::Stringify() and no length limit
#define OK_DECLARE_SV_DSTRUL_IPB_ENTRY(EntryName, ValueType, ExtensionName) \
  OK_DECLARE_SV_ENTRY_IMPL( \
    EntryName, \
    ValueType, \
    ::ok::StringifyNoLimit, \
    OK_SVMAP_NOT_IPBSERIALIZABLE())

// An entry that can be serialized to DynData
// with ok::Stringify() and standard length limit
#define OK_DECLARE_SV_DSTR_PBDYNDATA_ENTRY(EntryName, ValueType) \
  OK_DECLARE_SV_ENTRY_IMPL( \
    EntryName, \
    ValueType, \
    ::ok::Stringify, \
    OK_SVMAP_NOT_IPBSERIALIZABLE())

// An entry that can be serialized to DynData
// with ok::Stringify() and no length limit
#define OK_DECLARE_SV_DSTRUL_PBDYNDATA_ENTRY(EntryName, ValueType) \
  OK_DECLARE_SV_ENTRY_IMPL( \
    EntryName, \
    ValueType, \
    ::ok::StringifyNoLimit, \
    OK_SVMAP_NOT_IPBSERIALIZABLE())

// An entry that can be serialized to DynData
// with ok::Stringify() and standard length limit
#define OK_DECLARE_SV_DSTR_PBDYNDATA_ENTRY(EntryName, ValueType) \
  OK_DECLARE_SV_ENTRY_IMPL( \
    EntryName, \
    ValueType, \
    ::ok::Stringify, \
    OK_SVMAP_NOT_IPBSERIALIZABLE())

// An entry that can be serialized to DynData (array)
// with ok::Stringify() and no length limit
#define OK_DECLARE_SV_DSTRUL_PBDYNARR_ENTRY(EntryName, ValueType) \
  OK_DECLARE_SV_ENTRY_IMPL( \
    EntryName, \
    ValueType, \
    ::ok::StringifyNoLimit, \
    OK_SVMAP_NOT_IPBSERIALIZABLE())

// An IPBSerializable<DynamicProto> entry (that can be serialized to
// SVMap's DynData).  Using ok::Stringify() and no length limit
#define OK_DECLARE_SV_DSTRUL_IPB_OF_DYNDATA_ENTRY(EntryName, ValueType) \
  OK_DECLARE_SV_ENTRY_IMPL( \
    EntryName, \
    ValueType, \
    ::ok::StringifyNoLimit, \
    OK_SVMAP_NOT_IPBSERIALIZABLE())

#endif /* OK_ENABLE_PROTOBUF */



#endif /* OK_SV_SVCOMMON_HPP_ */
