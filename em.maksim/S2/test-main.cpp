#define BOOST_TEST_MODULE MainLogicTests
#include <boost/test/unit_test.hpp>
#include "transpose.hpp"

BOOST_AUTO_TEST_SUITE(transpose_tests)

BOOST_AUTO_TEST_CASE(empty_input) {
  em::SequenceList sequences;
  em::TransposeResult result =
    em::transpose_and_xor(sequences);
  BOOST_CHECK(result.transposed.empty());
  BOOST_CHECK(result.xor_results.empty());
}

BOOST_AUTO_TEST_CASE(single_sequence) {
  em::SequenceList sequences;
  em::IntList nums;
  nums.push_back(1);
  nums.push_back(2);
  nums.push_back(3);
  sequences.push_back(
    std::make_pair(std::string("first"), nums)
  );

  em::TransposeResult result =
    em::transpose_and_xor(sequences);
  BOOST_CHECK_EQUAL(result.transposed.size(), 3u);
  BOOST_CHECK_EQUAL(result.xor_results.size(), 3u);

  auto it = result.transposed.cbegin();
  BOOST_CHECK_EQUAL(it->size(), 1u);
  BOOST_CHECK_EQUAL(*it->cbegin(), 1);
  ++it;
  BOOST_CHECK_EQUAL(*it->cbegin(), 2);
  ++it;
  BOOST_CHECK_EQUAL(*it->cbegin(), 3);

  auto xor_it = result.xor_results.cbegin();
  BOOST_CHECK_EQUAL(*xor_it, 1);
  ++xor_it;
  BOOST_CHECK_EQUAL(*xor_it, 2);
  ++xor_it;
  BOOST_CHECK_EQUAL(*xor_it, 3);
}

BOOST_AUTO_TEST_CASE(multiple_sequences_different_length) {
  em::SequenceList sequences;

  em::IntList nums1;
  nums1.push_back(1);
  nums1.push_back(1);
  nums1.push_back(1);
  sequences.push_back(
    std::make_pair(std::string("first"), nums1)
  );

  em::IntList nums2;
  nums2.push_back(2);
  nums2.push_back(2);
  nums2.push_back(2);
  nums2.push_back(2);
  sequences.push_back(
    std::make_pair(std::string("second"), nums2)
  );

  em::IntList nums3;
  sequences.push_back(
    std::make_pair(std::string("third"), nums3)
  );

  em::IntList nums4;
  nums4.push_back(4);
  nums4.push_back(4);
  sequences.push_back(
    std::make_pair(std::string("fourth"), nums4)
  );

  em::TransposeResult result =
    em::transpose_and_xor(sequences);
  BOOST_CHECK_EQUAL(result.transposed.size(), 4u);
  BOOST_CHECK_EQUAL(result.xor_results.size(), 4u);

  auto xor_it = result.xor_results.cbegin();
  BOOST_CHECK_EQUAL(*xor_it, 7);
  ++xor_it;
  BOOST_CHECK_EQUAL(*xor_it, 7);
  ++xor_it;
  BOOST_CHECK_EQUAL(*xor_it, 3);
  ++xor_it;
  BOOST_CHECK_EQUAL(*xor_it, 2);
}

BOOST_AUTO_TEST_CASE(empty_sequences_only) {
  em::SequenceList sequences;

  em::IntList nums1;
  sequences.push_back(
    std::make_pair(std::string("first"), nums1)
  );

  em::IntList nums2;
  sequences.push_back(
    std::make_pair(std::string("second"), nums2)
  );

  BOOST_CHECK_THROW(
    em::transpose_and_xor(sequences),
    std::logic_error
  );
}

BOOST_AUTO_TEST_SUITE_END()
