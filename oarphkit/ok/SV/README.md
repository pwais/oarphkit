# SV: String-Value (or str -> void *) Data Structures

This module contains two data structures: SVMap and SVStruct.  Both 
structures allow for the definition of a map-like object where the
object's schema is *decentralized*.

In Python and JavaScript, a common practice is to treat dicts and JSONs as 
objects with 'dynamic' schemas (that are defined *implicitly* in their usage).
E.g. a function may operate on a specific set of dict/JSON key-value entries
instead of the properties of an explicitly declared object.  This practice
is particularly appealing because:

  * Data schemas often change rapidly in the early stages of a project.
  * dicts and JSONs are (typically) trivially serializable
  * dicts and JSONs enjoy related map-specific features, such as iteration
  * Recovery from malformed data and validation are possible, though at
     the expense of both dev time (more code) and runtime performance

C++ does not have a similar facility.  SVMap and SVStruct aim to bring many
of these conveniences to C++.

The core concrete difference between SVMap and SVStruct is that SVMap keys are
string *instances* while SVStruct keys are string *pointers*.  SVMap allows
one to assign arbitrary names to entries, while SVStruct entries are
unique and best interpreted as a set of *object attributes*. SVMap is closer
to a Python dict or a JSON Map; SVStruct is closer to a Python object.

One feature related to this topic is Categories in Objective-C.  Categories
allow for source-file-local customization of an object's schema and interface.
C++ provides no means for effectively distributing the schema definition of an
object among source files.  SVMap and SVStruct indirectly provide such a
feature since code modules may separately define their own {SVMap,SVStruct} 
entries associated with instances that the modules share in common.

Finally, SVMap and SVStruct (and the rest of OarphKit) aim to make data
serialization easy (yet flexible) in C++.  Entries must define their
own serialization, but {SVMap,SVStruct} take care of the rest (and ignore
values that don't support serialization).
  