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
#ifndef OK_BRANCHPREDICT_HPP_
#define OK_BRANCHPREDICT_HPP_

#if __GNUC__ || __clang__
#define OK_LIKELY(c) __builtin_expect(bool(c), true)
#define OK_UNLIKELY(c) __builtin_expect(bool(c), false)
#else
#define OK_LIKELY(c) c
#define OK_UNLIKELY(c) c
#endif

#define OK_UNLIKELY_DO(cond, exp) do { \
    if(OK_UNLIKELY(cond)) { exp; } \
  } while(0);

#endif /* OK_BRANCHPREDICT_HPP_ */
