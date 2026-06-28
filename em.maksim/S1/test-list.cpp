#define BOOST_TEST_MODULE ListTests
#include <boost/test/included/unit_test.hpp>
#include "List.h"

BOOST_AUTO_TEST_CASE(push_operations) {
  em::List<int> list;
  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);

  list.push_back(10);
  list.push_front(5);

  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(list.front(), 5);
  BOOST_CHECK_EQUAL(list.back(), 10);
}

BOOST_AUTO_TEST_CASE(pop_operations) {
  em::List<int> list;
  list.push_back(10);
  list.push_front(5);
  list.push_back(20);

  list.pop_front();
  BOOST_CHECK_EQUAL(list.front(), 10);
  BOOST_CHECK_EQUAL(list.size(), 2);

  list.pop_back();
  BOOST_CHECK_EQUAL(list.back(), 10);
  BOOST_CHECK_EQUAL(list.size(), 1);

  list.pop_front();
  BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE(iterator_traversal) {
  em::List<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  int expected = 1;
  for (auto it = list.begin(); it != list.end(); ++it) {
    BOOST_CHECK_EQUAL(*it, expected);
    ++expected;
  }
  BOOST_CHECK_EQUAL(expected, 4);
}

BOOST_AUTO_TEST_CASE(copy_constructor) {
  em::List<int> original;
  original.push_back(100);
  original.push_back(200);

  em::List<int> copy(original);

  BOOST_CHECK_EQUAL(copy.size(), 2);
  BOOST_CHECK_EQUAL(copy.front(), 100);
  BOOST_CHECK_EQUAL(copy.back(), 200);

  original.pop_front();
  BOOST_CHECK_EQUAL(original.size(), 1);
  BOOST_CHECK_EQUAL(copy.size(), 2);
}

BOOST_AUTO_TEST_CASE(move_constructor) {
  em::List<int> original;
  original.push_back(42);

  em::List<int> moved(std::move(original));

  BOOST_CHECK_EQUAL(moved.front(), 42);
  BOOST_CHECK(original.empty());
}

BOOST_AUTO_TEST_CASE(empty_list_access) {
  em::List<int> list;
  BOOST_CHECK_THROW(list.front(), std::out_of_range);
  BOOST_CHECK_THROW(list.back(), std::out_of_range);
}
