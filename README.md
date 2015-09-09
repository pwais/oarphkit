# OarphKit
                 __________________________
                < OarphKit!! Oarph! Oarph! >
                <       with Whale Test!!  >
                 --------------------------
                   /
                  /
            -~~~~-
           |O __ O|
        -__|_\__/_|__-
        ---(__/\__)---
          /  ~--~  \
         /  | \/ |  \
        |   | /\ |   |
      _/ /\__    __/\ \_
    ~~~~~    ~~~~    ~~~~~

[![License](http://img.shields.io/:license-apache-orange.svg)](http://www.apache.org/licenses/LICENSE-2.0)

OarphKit is a collection of C++ utilities with an emphasis on portability.

Highlights:

  * [FLi: Functor (meta) Library](oarphkit/ok/fli/README.md). A framework for
      building portable Functor-based libraries.
  * [SVMap & SVStruct](oarphkit/ok/SV/README.md): A hashmap-based data structure
      that brings many of the architectural advantages of Python's `dict` to C++.
  * Serialization utilities (e.g. for interoperating with Google Protobuf).  A
      [DynamicProto](oarphkit/ok/SerializationUtils/DynamicProto/README.md) utility
      for Protobuf-compatible data that has an implicit (usage-defined) schema.
  * [IArray](oarphkit/ok/IArray.hpp): impute a standard array interface upon any
      buffer.
  * [Stringify](oarphkit/ok/Stringify.hpp): Pretty-print anything.
  * Concise, single-header [assert](oarphkit/ok/OKAssert.hpp) and
      [logging](oarphkit/ok/OKLog.hpp) utilities.
  * A variety of [compile-time flags](CMakeLists.txt) that trade between
      safety and performance.  The `$ ./bootstrap.py --whale-test` feature
      ensures the correctness of unit tests under all `2^N` combinations of
      flags.

## Dev Quickstart

OarphKit supports Linux, Mac OS X, and similar platforms.  OarphKit does
*not* currently support Windows.  In a checkout of the repo, run
`$ ./bootstrap.py --all` to build in-place; try `$ ./bootstrap.py --indocker`
to build inside a container.

Note that Oarphkit uses cmake globs to identify compilable source files.  If
you add a file, you must `$ touch CMakeLists.txt` and re-generate your 
Makefiles (e.g. via `$ ./bootstrap.py --build`).

### Profiling

For Mac OS X, we recommend using valgrind with qcachegrind.  Use `--dsymutil=yes`
to ensure callgrind output includes useful debug symbol information.

```
$ sudo port install valgrind qcachegrind graphviz
$ ./bootstrap --build
$ valgrind --dsymutil=yes --tool=callgrind build/oarphkit_test
$ open qcachegrind  # Open the build/callgrind.out.$pid file generated
```

[license-url]: LICENSE
