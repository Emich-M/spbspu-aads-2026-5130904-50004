#define BOOST_TEST_MODULE BListTests
#include <boost/test/unit_test.hpp>
#include "BList.hpp"

BOOST_AUTO_TEST_SUITE(BList_basic)

BOOST_AUTO_TEST_CASE(default_constructor) {
  em::List<int> list;
  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0u);
}

BOOST_AUTO_TEST_CASE(push_back) {
  em::List<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  BOOST_CHECK_EQUAL(list.size(), 3u);
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 3);
}

BOOST_AUTO_TEST_CASE(push_front) {
  em::List<int> list;
  list.push_front(1);
  list.push_front(2);
  list.push_front(3);
  BOOST_CHECK_EQUAL(list.size(), 3u);
  BOOST_CHECK_EQUAL(list.front(), 3);
  BOOST_CHECK_EQUAL(list.back(), 1);
}

BOOST_AUTO_TEST_CASE(pop_front) {
  em::List<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.pop_front();
  BOOST_CHECK_EQUAL(list.size(), 2u);
  BOOST_CHECK_EQUAL(list.front(), 2);
}

BOOST_AUTO_TEST_CASE(pop_back) {
  em::List<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.pop_back();
  BOOST_CHECK_EQUAL(list.size(), 2u);
  BOOST_CHECK_EQUAL(list.back(), 2);
}

BOOST_AUTO_TEST_CASE(clear) {
  em::List<int> list;
  list.push_back(1);
  list.push_back(2);
  list.clear();
  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0u);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(BList_iterator)

BOOST_AUTO_TEST_CASE(iterate_forward) {
  em::List<int> list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);

  int sum = 0;
  for (auto it = list.cbegin(); it != list.cend(); ++it) {
    sum += *it;
  }
  BOOST_CHECK_EQUAL(sum, 60);
}

BOOST_AUTO_TEST_CASE(modify_through_iterator) {
  em::List<int> list;
  list.push_back(10);
  list.push_back(20);

  auto it = list.begin();
  *it = 100;
  BOOST_CHECK_EQUAL(list.front(), 100);
}

BOOST_AUTO_TEST_CASE(iterate_backward) {
  em::List<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  auto it = --list.end();
  BOOST_CHECK_EQUAL(*it, 3);
  --it;
  BOOST_CHECK_EQUAL(*it, 2);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(BList_copy_move)

BOOST_AUTO_TEST_CASE(copy_constructor) {
  em::List<int> list1;
  list1.push_back(1);
  list1.push_back(2);

  em::List<int> list2 = list1;
  BOOST_CHECK_EQUAL(list2.size(), 2u);
  BOOST_CHECK_EQUAL(list2.front(), 1);
  BOOST_CHECK_EQUAL(list2.back(), 2);

  list1.push_back(3);
  BOOST_CHECK_EQUAL(list1.size(), 3u);
  BOOST_CHECK_EQUAL(list2.size(), 2u);
}

BOOST_AUTO_TEST_CASE(move_constructor) {
  em::List<int> list1;
  list1.push_back(1);
  list1.push_back(2);

  em::List<int> list2 = std::move(list1);
  BOOST_CHECK_EQUAL(list2.size(), 2u);
  BOOST_CHECK(list1.empty());
}

BOOST_AUTO_TEST_SUITE_END()
