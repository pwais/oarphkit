# FLi Runtime Library

This module contains components for building, executing, and (de)serializing
a FLi program.  Key concepts:

  * A [Registry](Registry.hpp) that provides a very basic reflection (as in
      Java Reflection) feature for FLi.  A `Registry` is concretely a mapping
      between Functor factory methods and strings (`alias`es) that can be used
      as tokens in serialzed data.

  * An [env](env.hpp) is an Environment: the root scope for a set of variable
      assignments. `env` is concretely a hashmap of string -> object. An
      `env` stores a root `Registry`, all Functor instances, and may be
      (de)serialized and merged with another `env`.  The purpose of an `env`
      is to encapsulate a FLi program's state and enable serialization
      of the program.
  
  * A [Session](Session.hpp) wraps a single `env` instance and provides a
      user-oriented interface for running and interoperating with a FLi
      program.
  
  * This module contains other utilities for serializing Functors. 
