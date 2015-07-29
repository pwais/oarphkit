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
#ifndef OK_TEST_OKTASSERTIONS_HPP_
#define OK_TEST_OKTASSERTIONS_HPP_

#include "gtest/gtest.h"

#include <algorithm>

namespace ok_test {

// GTest appears to lack a sequence checker... so here is one!
template <typename ExpectedBT,
          typename ExpectedET,
          typename ActualBT,
          typename ActualET>
::testing::AssertionResult AssertRangesEqual(
    const ExpectedBT &expected_begin,
    const ExpectedET &expected_end,
    const ActualBT &actual_begin,
    const ActualET &actual_end,
    const char *expected_name,
    const char *actual_name) {
  const size_t size_expected = expected_end - expected_begin;
  const size_t size_actual = actual_end - actual_begin;
  if (size_expected != size_actual) {
    return
        ::testing::AssertionFailure() <<
        "Size mistmatch: " <<
        expected_name << " (size " << size_expected << ") != " <<
        actual_name << " (size " << size_actual << ")";
  }

  const size_t kMaxMismatchesToPrint = 5;
  size_t num_mismatches = 0;
  auto failure = ::testing::AssertionFailure();
  failure <<
      "Sequences ( " << expected_name << " vs " << actual_name <<
      " ) mismatch: ";
  for (size_t i = 0; i < size_expected; ++i) {
    const auto& expected_val = *(expected_begin + i);
    const auto& actual_val = *(actual_begin + i);
    if (expected_val != actual_val) {
      ++num_mismatches;
      if (num_mismatches < kMaxMismatchesToPrint) {
        failure << "i=" << i << " (expected: " << expected_val << " vs " <<
            "actual: " << actual_val << ") ";
      }
    }
  }

  if (num_mismatches == 0) {
    return ::testing::AssertionSuccess();
  }

  if (num_mismatches >= kMaxMismatchesToPrint) {
    failure << "... ";
  }
  failure << "(total: " << num_mismatches << ")";
  return failure;
}

template <typename ExpectedT, typename ActualT>
::testing::AssertionResult AssertSequencesEqual(
    const ExpectedT &expected,
    const ActualT &actual,
    const char *expected_name,
    const char *actual_name) {

  return AssertRangesEqual(
      expected.begin(),
      expected.end(),
      actual.begin(),
      actual.end(),
      expected_name,
      actual_name);
}

} /* namespace oktest */

#define EXPECT_SEQUENCES_EQUAL(expected, actual) \
  EXPECT_TRUE( \
    ::ok_test::AssertSequencesEqual( \
      expected, \
      actual, \
      #expected, \
      #actual)); \

#define EXPECT_RANGES_EQUAL(expected_begin, expected_end, actual_begin, actual_end) \
  EXPECT_TRUE( \
    ::ok_test::AssertRangesEqual( \
      expected_begin, \
      expected_end, \
      actual_begin, \
      actual_end, \
      #expected_begin, \
      #actual_begin)); \

#define EXPECT_SORTED_SEQUENCES_EQUAL(expected, actual) do { \
    auto e2 = expected; \
    auto a2 = actual; \
    std::sort(e2.begin(), e2.end()); \
    std::sort(a2.begin(), a2.end()); \
    EXPECT_TRUE( \
      ::ok_test::AssertSequencesEqual( \
        e2, \
        a2, \
        #expected, \
        #actual)); \
  } while(0)

#endif /* OK_TEST_OKTASSERTIONS_HPP_ */
