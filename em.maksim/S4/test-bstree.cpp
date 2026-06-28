#include "bstree.hpp"

#include <string>
#include <utility>
#include <vector>
#include <functional>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(BSTreeSuite)

BOOST_AUTO_TEST_CASE(default_construction)
{
  em::BSTree<int, std::string, std::less<int>> tree;

  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.size() == 0);
  BOOST_TEST(tree.height() == 0);
  BOOST_TEST(tree.begin() == tree.end());
}

BOOST_AUTO_TEST_CASE(push_and_get)
{
  em::BSTree<int, std::string, std::less<int>> tree;

  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(0, "zero");

  BOOST_TEST(tree.get(0) == "zero");
  BOOST_TEST(tree.get(1) == "one");
  BOOST_TEST(tree.get(2) == "two");
  BOOST_TEST(tree.size() == 3);
}

BOOST_AUTO_TEST_CASE(push_duplicate_key_updates_value)
{
  em::BSTree<int, std::string, std::less<int>> tree;

  tree.push(10, "ten");
  tree.push(10, "TEN");

  BOOST_TEST(tree.get(10) == "TEN");
  BOOST_TEST(tree.size() == 1);
}

BOOST_AUTO_TEST_CASE(get_missing_key_throws)
{
  em::BSTree<int, double, std::less<int>> tree;

  tree.push(5, 3.14);

  BOOST_CHECK_THROW(tree.get(4), std::out_of_range);
  BOOST_CHECK_THROW(tree.get(6), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(drop_existing_key)
{
  em::BSTree<char, int, std::less<char>> tree;

  tree.push('a', 100);
  tree.push('b', 200);
  tree.push('c', 300);

  int removed = tree.drop('b');

  BOOST_TEST(removed == 200);
  BOOST_TEST(tree.size() == 2);
  BOOST_TEST(tree.get('a') == 100);
  BOOST_TEST(tree.get('c') == 300);
  BOOST_CHECK_THROW(tree.get('b'), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(drop_missing_key_throws)
{
  em::BSTree<int, std::string, std::less<int>> tree;

  tree.push(4, "four");

  BOOST_CHECK_THROW(tree.drop(2), std::out_of_range);
  BOOST_CHECK_THROW(tree.drop(6), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(iterator_traversal)
{
  em::BSTree<int, std::string, std::less<int>> tree;

  tree.push(3, "three");
  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(4, "four");

  std::vector<std::pair<int, std::string>> items;
  for (auto it = tree.begin(); it != tree.end(); ++it) {
    items.emplace_back(it->first, it->second);
  }

  BOOST_REQUIRE(items.size() == 4);
  BOOST_TEST(items[0].first == 1);
  BOOST_TEST(items[0].second == "one");
  BOOST_TEST(items[1].first == 2);
  BOOST_TEST(items[1].second == "two");
  BOOST_TEST(items[2].first == 3);
  BOOST_TEST(items[2].second == "three");
  BOOST_TEST(items[3].first == 4);
  BOOST_TEST(items[3].second == "four");
}

BOOST_AUTO_TEST_CASE(const_iterator_and_conversion)
{
  em::BSTree<std::string, double, std::less<std::string>> tree;

  tree.push("e", 2.71);
  tree.push("pi", 3.14);

  std::vector<std::string> keys;
  for (auto ci = tree.cbegin(); ci != tree.cend(); ++ci) {
    keys.push_back(ci->first);
  }

  BOOST_REQUIRE(keys.size() == 2);
  BOOST_TEST(keys[0] == "e");
  BOOST_TEST(keys[1] == "pi");

  const auto &ctree = tree;
  BOOST_TEST(ctree.begin() == ctree.cbegin());
  BOOST_TEST(ctree.end() == ctree.cend());
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  em::BSTree<int, int, std::less<int>> tree;

  tree.push(10, 100);
  tree.push(20, 200);
  tree.push(30, 300);

  em::BSTree<int, int, std::less<int>> moved(std::move(tree));

  BOOST_TEST(moved.get(10) == 100);
  BOOST_TEST(moved.get(20) == 200);
  BOOST_TEST(moved.get(30) == 300);
  BOOST_TEST(moved.size() == 3);

  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.height() == 0);
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  em::BSTree<int, int, std::less<int>> tree1;
  tree1.push(10, 100);
  tree1.push(20, 200);

  em::BSTree<int, int, std::less<int>> tree2;
  tree2.push(30, 300);

  tree2 = std::move(tree1);

  BOOST_TEST(tree2.get(10) == 100);
  BOOST_TEST(tree2.get(20) == 200);
  BOOST_TEST(tree2.size() == 2);
  BOOST_TEST(tree1.empty());
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  em::BSTree<int, std::string, std::less<int>> tree;
  tree.push(1, "one");
  tree.push(2, "two");

  em::BSTree<int, std::string, std::less<int>> copied(tree);

  BOOST_TEST(copied.get(1) == "one");
  BOOST_TEST(copied.get(2) == "two");
  BOOST_TEST(copied.size() == 2);

  BOOST_TEST(tree.get(1) == "one");
  BOOST_TEST(tree.size() == 2);
}

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  em::BSTree<int, std::string, std::less<int>> tree1;
  tree1.push(1, "one");

  em::BSTree<int, std::string, std::less<int>> tree2;
  tree2.push(2, "two");

  tree2 = tree1;

  BOOST_TEST(tree2.get(1) == "one");
  BOOST_TEST(tree2.size() == 1);
  BOOST_TEST(tree1.get(1) == "one");
}

BOOST_AUTO_TEST_CASE(rotate_left_keeps_order)
{
  em::BSTree<int, char, std::less<int>> tree;

  tree.push(2, 'b');
  tree.push(1, 'a');
  tree.push(3, 'c');

  auto it = tree.begin();
  ++it;

  tree.rotateLeft(it);

  std::vector<int> keys;
  for (auto ci = tree.cbegin(); ci != tree.cend(); ++ci) {
    keys.push_back(ci->first);
  }

  BOOST_REQUIRE(keys.size() == 3);
  BOOST_TEST(keys[0] == 1);
  BOOST_TEST(keys[1] == 2);
  BOOST_TEST(keys[2] == 3);
}

BOOST_AUTO_TEST_CASE(rotate_right_keeps_order)
{
  em::BSTree<int, char, std::less<int>> tree;

  tree.push(2, 'b');
  tree.push(1, 'a');
  tree.push(3, 'c');

  auto it = tree.begin();
  ++it;

  tree.rotateRight(it);

  std::vector<int> keys;
  for (auto ci = tree.cbegin(); ci != tree.cend(); ++ci) {
    keys.push_back(ci->first);
  }

  BOOST_REQUIRE(keys.size() == 3);
  BOOST_TEST(keys[0] == 1);
  BOOST_TEST(keys[1] == 2);
  BOOST_TEST(keys[2] == 3);
}

BOOST_AUTO_TEST_CASE(height_calculation)
{
  em::BSTree<int, std::string, std::less<int>> tree;

  BOOST_TEST(tree.height() == 0);

  tree.push(2, "two");
  BOOST_TEST(tree.height() == 1);

  tree.push(1, "one");
  BOOST_TEST(tree.height() == 2);

  tree.push(3, "three");
  BOOST_TEST(tree.height() >= 2);
}

BOOST_AUTO_TEST_CASE(iterator_increment_decrement)
{
  em::BSTree<int, std::string, std::less<int>> tree;

  tree.push(3, "three");
  tree.push(1, "one");
  tree.push(2, "two");

  auto it = tree.begin();
  BOOST_TEST(it->first == 1);

  ++it;
  BOOST_TEST(it->first == 2);

  ++it;
  BOOST_TEST(it->first == 3);

  --it;
  BOOST_TEST(it->first == 2);

  --it;
  BOOST_TEST(it->first == 1);
}

BOOST_AUTO_TEST_CASE(empty_tree_operations)
{
  em::BSTree<int, std::string, std::less<int>> tree;

  BOOST_TEST(tree.begin() == tree.end());
  BOOST_TEST(tree.cbegin() == tree.cend());
  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.size() == 0);

  tree.clear();
  BOOST_TEST(tree.empty());
}

BOOST_AUTO_TEST_CASE(large_tree)
{
  em::BSTree<int, int, std::less<int>> tree;

  const int size = 100;
  for (int i = 0; i < size; ++i) {
    tree.push(i, i * 10);
  }

  BOOST_TEST(tree.size() == size);

  for (int i = 0; i < size; ++i) {
    BOOST_TEST(tree.get(i) == i * 10);
  }

  int count = 0;
  for (auto it = tree.begin(); it != tree.end(); ++it) {
    BOOST_TEST(it->second == it->first * 10);
    ++count;
  }
  BOOST_TEST(count == size);
}

BOOST_AUTO_TEST_SUITE_END()
