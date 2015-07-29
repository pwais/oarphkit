# FLi Core

This module contains FLi's core components, including:
  
  * (Core)[Core.hpp] -- Core code defining typechecks, run-time- and 
      compile-time-checkable data (with elidable checks), and (of course)
      the base `FunctorBase` class. 
 
  * (TPtr)[TPtr.hpp] -- A unique_ptr<T> that may serve as a weak reference.
      Inspired by kj::Own<T>, `TPtr` is designed for similicity and easy
      interoperation with STL pointers utilities.  See the `TPtr` header for
      discussion.
  
  * Functors (dir) -- A collection of Functor Prototypes to serve as base
      classes for user code.  Includes a (utility)[Functors/FuncWrapper.hpp]
      for wrapping ordinary C++ functions.
