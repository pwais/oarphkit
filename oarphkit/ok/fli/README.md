# FLi: Functor (meta) Library

This component contains a framework for writing Functor-based libraries
with an emphasis on the portability of functor-based programs.  The
framework helps the library writer expose a set of native functors--
and *compositions* of those functors -- to client code and ensure
native behavior is consistent (and performant) across client platforms.

For an overview of key components, see:

  * [Runtime](Runtime/README.md) -- Library user interface for building,
      running, and serializing FLi programs.
  * [Core](Core/README.md) -- Most basic FLi Library building blocks.
  * [FSL](fsl/README.md) -- A suite of standard FLi Functors.

# Compile-time Macro Options

See `FLiConfig.hpp` for compile-time macro-based library options (e.g. to
disable runtime typechecks for speed).  Note that FLi unit tests are designed
to correctly verify FLi library behavior under the various combinations of
these flags.  Try OarphKit's `$ bootstrap.py --whale-test` to exhaustively
test flag combinations and verify library integrity.

# Debugging

## Tools

Try using the OarphKit `flix` executable to run and inspect FLi programs
in your terminal.  See `$ bootstrap.py --test-flix` and the root `flix`
directory.

## Types

If your compiler cannot support type demangling, try using `c++filt`
on mangled type names:

  ```
  $ c++filt -t Dn
  std::nullptr_t
  ```

# References

A variety of libraries have inspired the design of FLi, including (but not
limited to):

  * cling - https://github.com/vgvassilev/cling
  * djinni - https://github.com/dropbox/djinni
  * JNA - https://github.com/twall/jna
  * SWIG - http://www.swig.org/
