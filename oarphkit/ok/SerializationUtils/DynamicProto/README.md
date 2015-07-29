# DynamicProto

A flexible and portable way to encode schemaless data using Protobuf. This 
implementation is compatible with older versions of Protobuf and this
package provides utilities that fail gracefully in the absence of
expected data (or of Protobuf itself).  If Protobuf is *not* available, then
utility methods behave as if the DynamicProto instance is empty.