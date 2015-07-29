// TODO deleteme
///*
// * Copyright 2015 Maintainers of OarphKit
// *
// * Licensed under the Apache License, Version 2.0 (the "License");
// * you may not use this file except in compliance with the License.
// * You may obtain a copy of the License at
// *
// *     http://www.apache.org/licenses/LICENSE-2.0
// *
// * Unless required by applicable law or agreed to in writing, software
// * distributed under the License is distributed on an "AS IS" BASIS,
// * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// * See the License for the specific language governing permissions and
// * limitations under the License.
// */
//#ifndef OK_FLI_SESSION_HPP_
//#define OK_FLI_SESSION_HPP_
//
//#include "ok/fli/Core/Core.hpp"
//
//namespace ok { namespace fli {
//
///**
// *
// */
//
////class Session {
////public:
////  Session() { }
////  ~Session() { }
////
////  typedef std::unique_ptr<Session> Ptr;
////
////  static Ptr CreateFromSpec(...);
////
////
////  void Run();
////
////};
//
///*
// * Goal: we can serialize a program and run re-run anywhere:
// *  * session has one env, can serialize all or parts
// *  * there are functor instances (in env?), and Session can
// *       remember functor spec and save as part of session ...
// *
// *
// *  ..
// *
// *  env:
// *    * registry -> obj
// *           name -> func
// *           funcs: []
// *           *** we need funcs to be serializable ...
// *                  either store the spec or store the state .. ?
// *    * vars: key -> value
// *    *  ... scopes? name -> svmap. or util for str name -> SVMap.value
// *
// *
// *    env: (name, value, serializer)
// *       * FunctorBase::Save(ser_ctx)
// *           ser_ctx::Save(IArray<byte>)
// *           ser_ctx::Save(pb)
// *           *** protobuf: can add data thru extensions!
// *            so user defines pb data and then have to give to Save ...
// *            need to give pb instance to Save?
// *
// *            ideally:
// *
// *       *
// *
// *
// *
// *
// * events:
// *  onInit
// *  onRun
// *  onRunComplete
// *  onTeardown
// *
// *  Call <-- Session is a Functor ...
// *
// *
// *  Run() -- Call() in /loop/ until something pushes exit ?
// *
// *  StreamSession --
// *  Push()
// *  Pop() ..
// *  map(functor, Iter) <-- iter could be a Source
// *                         functor could include a Sink
// *
// *  reduce ->> is a functor with a Sink ...
// *
// * ** streams / map would allow nice threadpool parallelism
// *
// *
// * minibatch -- might need to iter over data multiple times!
// *
// *
// * state is k-v
// *
// * while ( bool Src )
// *   Subroutine
// *
// * state[outN: list] = map (functor, Src)
// *
// * state[outN: val] = reduce : functor (Src->val) [call once]
// *
// *
// */
//
//} /* namespace fli */
//} /* namespace ok */
//
//#endif /* OK_FLI_SESSION_HPP_ */
