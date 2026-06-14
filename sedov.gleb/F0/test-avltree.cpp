#include <boost/test/unit_test.hpp>
#include <string>
#include <stdexcept>
#include <utility>
#include "avltree.hpp"
#include "../common/vector.hpp"

using namespace sedov;

BOOST_AUTO_TEST_SUITE(AVLTreeTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  AVLTree< int, std::string > tree;
  BOOST_CHECK(tree.empty());
  BOOST_CHECK_EQUAL(tree.size(), 0);
  BOOST_CHECK(tree.begin() == tree.end());
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  AVLTree< int, std::string > tree1;
  tree1.insert(1, "one");
  tree1.insert(2, "two");
  tree1.insert(3, "three");
  AVLTree< int, std::string > tree2(tree1);
  BOOST_CHECK_EQUAL(tree2.size(), 3);
  BOOST_CHECK_EQUAL(tree2.at(1), "one");
  BOOST_CHECK_EQUAL(tree2.at(2), "two");
  BOOST_CHECK_EQUAL(tree2.at(3), "three");
  tree2.at(1) = "ONE";
  BOOST_CHECK_EQUAL(tree1.at(1), "one");
  BOOST_CHECK_EQUAL(tree2.at(1), "ONE");
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  AVLTree< int, std::string > tree1;
  tree1.insert(1, "one");
  tree1.insert(2, "two");
  AVLTree< int, std::string > tree2(std::move(tree1));
  BOOST_CHECK_EQUAL(tree2.size(), 2);
  BOOST_CHECK_EQUAL(tree2.at(1), "one");
  BOOST_CHECK_EQUAL(tree2.at(2), "two");
  BOOST_CHECK(tree1.empty());
}

BOOST_AUTO_TEST_CASE(test_copy_assignment)
{
  AVLTree< int, std::string > tree1;
  tree1.insert(1, "one");
  AVLTree< int, std::string > tree2;
  tree2.insert(99, "ninety-nine");
  tree2 = tree1;
  BOOST_CHECK_EQUAL(tree2.size(), 1);
  BOOST_CHECK_EQUAL(tree2.at(1), "one");
  BOOST_CHECK_THROW(tree2.at(99), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_move_assignment)
{
  AVLTree< int, std::string > tree1;
  tree1.insert(1, "one");

  AVLTree< int, std::string > tree2;
  tree2.insert(99, "ninety-nine");
  tree2 = std::move(tree1);
  BOOST_CHECK_EQUAL(tree2.size(), 1);
  BOOST_CHECK_EQUAL(tree2.at(1), "one");
  BOOST_CHECK(tree1.empty());
}

BOOST_AUTO_TEST_CASE(test_swap)
{
  AVLTree< int, std::string > tree1;
  tree1.insert(1, "one");
  AVLTree< int, std::string > tree2;
  tree2.insert(2, "two");
  tree2.insert(3, "three");
  tree1.swap(tree2);
  BOOST_CHECK_EQUAL(tree1.size(), 2);
  BOOST_CHECK_EQUAL(tree1.at(2), "two");
  BOOST_CHECK_EQUAL(tree2.size(), 1);
  BOOST_CHECK_EQUAL(tree2.at(1), "one");
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  AVLTree< int, std::string > tree;
  tree.insert(1, "one");
  tree.insert(2, "two");
  tree.insert(3, "three");
  tree.clear();
  BOOST_CHECK(tree.empty());
  BOOST_CHECK_EQUAL(tree.size(), 0);
  BOOST_CHECK_THROW(tree.at(1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_insert_new_keys)
{
  AVLTree< int, std::string > tree;
  tree.insert(1, "one");
  BOOST_CHECK_EQUAL(tree.size(), 1);
  tree.insert(2, "two");
  BOOST_CHECK_EQUAL(tree.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_insert_update_existing)
{
  AVLTree< int, std::string > tree;
  tree.insert(1, "one");
  tree.insert(1, "ONE");
  BOOST_CHECK_EQUAL(tree.size(), 1);
  BOOST_CHECK_EQUAL(tree.at(1), "ONE");
}

BOOST_AUTO_TEST_CASE(test_insert_rvalue)
{
  AVLTree< int, std::string > tree;
  std::string val = "rvalue";
  tree.insert(1, std::move(val));
  BOOST_CHECK_EQUAL(tree.at(1), "rvalue");
}

BOOST_AUTO_TEST_CASE(test_push_alias)
{
  AVLTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");
  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK_EQUAL(tree.at(1), "one");
  BOOST_CHECK_EQUAL(tree.at(2), "two");
}

BOOST_AUTO_TEST_CASE(test_push_rvalue)
{
  AVLTree< int, std::string > tree;
  std::string val = "rvalue";
  tree.push(1, std::move(val));
  BOOST_CHECK_EQUAL(tree.at(1), "rvalue");
}

BOOST_AUTO_TEST_CASE(test_find_existing)
{
  AVLTree< int, std::string > tree;
  tree.insert(1, "one");
  tree.insert(2, "two");
  std::string value;
  tree.find(2, value);
  BOOST_CHECK_EQUAL(value, "two");
}

BOOST_AUTO_TEST_CASE(test_find_non_existing)
{
  AVLTree< int, std::string > tree;
  tree.insert(1, "one");
  std::string value;
  bool found = tree.find(999, value);
  BOOST_CHECK(!found);
}

BOOST_AUTO_TEST_CASE(test_contains)
{
  AVLTree< int, std::string > tree;
  tree.insert(1, "one");
  tree.insert(2, "two");
  BOOST_CHECK(tree.contains(1));
  BOOST_CHECK(tree.contains(2));
  BOOST_CHECK(!tree.contains(3));
}

BOOST_AUTO_TEST_CASE(test_at_const)
{
  AVLTree< int, std::string > tree;
  tree.insert(1, "one");
  tree.insert(2, "two");
  const AVLTree< int, std::string > & constTree = tree;
  BOOST_CHECK_EQUAL(constTree.at(1), "one");
  BOOST_CHECK_EQUAL(constTree.at(2), "two");
}

BOOST_AUTO_TEST_CASE(test_at_non_const)
{
  AVLTree< int, std::string > tree;
  tree.insert(1, "one");
  tree.at(1) = "ONE";
  BOOST_CHECK_EQUAL(tree.at(1), "ONE");
}

BOOST_AUTO_TEST_CASE(test_at_not_found)
{
  AVLTree< int, std::string > tree;
  tree.insert(1, "one");
  BOOST_CHECK_THROW(tree.at(2), std::out_of_range);
  const AVLTree< int, std::string > & constTree = tree;
  BOOST_CHECK_THROW(constTree.at(2), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_erase_leaf)
{
  AVLTree< int, std::string > tree;
  tree.insert(2, "two");
  tree.insert(1, "one");
  tree.insert(3, "three");
  tree.erase(1);
  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK(!tree.contains(1));
  BOOST_CHECK(tree.contains(2));
  BOOST_CHECK(tree.contains(3));
}

BOOST_AUTO_TEST_CASE(test_erase_node_with_one_child)
{
  AVLTree< int, std::string > tree;
  tree.insert(2, "two");
  tree.insert(1, "one");
  tree.erase(2);
  BOOST_CHECK_EQUAL(tree.size(), 1);
  BOOST_CHECK(!tree.contains(2));
  BOOST_CHECK(tree.contains(1));
}

BOOST_AUTO_TEST_CASE(test_erase_node_with_two_children)
{
  AVLTree< int, std::string > tree;
  tree.insert(2, "two");
  tree.insert(1, "one");
  tree.insert(3, "three");
  tree.erase(2);
  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK(!tree.contains(2));
  BOOST_CHECK(tree.contains(1));
  BOOST_CHECK(tree.contains(3));
}

BOOST_AUTO_TEST_CASE(test_erase_non_existing)
{
  AVLTree< int, std::string > tree;
  tree.insert(1, "one");
  tree.erase(999);
  BOOST_CHECK_EQUAL(tree.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_drop)
{
  AVLTree< int, std::string > tree;
  tree.insert(1, "one");
  tree.insert(2, "two");
  tree.insert(3, "three");
  std::string val = tree.drop(2);
  BOOST_CHECK_EQUAL(val, "two");
  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK(!tree.contains(2));
  BOOST_CHECK_EQUAL(tree.at(1), "one");
  BOOST_CHECK_EQUAL(tree.at(3), "three");
}

BOOST_AUTO_TEST_CASE(test_drop_nonexistent)
{
  AVLTree< int, std::string > tree;
  tree.insert(1, "one");
  BOOST_CHECK_THROW(tree.drop(999), std::out_of_range);
  BOOST_CHECK_EQUAL(tree.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_iterator_traversal)
{
  AVLTree< int, std::string > tree;
  tree.insert(3, "three");
  tree.insert(1, "one");
  tree.insert(2, "two");
  tree.insert(4, "four");
  Vector< std::pair< const int, std::string > > result;
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    result.pushBack(*it);
  }
  BOOST_REQUIRE_EQUAL(result.getSize(), 4);
  BOOST_CHECK_EQUAL(result[0].first, 1);
  BOOST_CHECK_EQUAL(result[1].first, 2);
  BOOST_CHECK_EQUAL(result[2].first, 3);
  BOOST_CHECK_EQUAL(result[3].first, 4);
}

BOOST_AUTO_TEST_CASE(test_const_iterator)
{
  AVLTree< int, std::string > tree;
  tree.insert(1, "one");
  tree.insert(2, "two");
  const AVLTree< int, std::string > & constTree = tree;
  Vector< int > keys;
  for (auto it = constTree.cbegin(); it != constTree.cend(); ++it)
  {
    keys.pushBack((*it).first);
  }
  BOOST_REQUIRE_EQUAL(keys.getSize(), 2);
  BOOST_CHECK_EQUAL(keys[0], 1);
  BOOST_CHECK_EQUAL(keys[1], 2);
}

BOOST_AUTO_TEST_CASE(test_iterator_equality)
{
  AVLTree< int, std::string > tree;
  tree.insert(1, "one");
  auto it1 = tree.begin();
  auto it2 = tree.begin();
  auto it3 = tree.end();
  BOOST_CHECK(it1 == it2);
  BOOST_CHECK(it1 != it3);
  BOOST_CHECK(tree.begin() != tree.end());
}

BOOST_AUTO_TEST_CASE(test_end_iterator)
{
  AVLTree< int, std::string > tree;
  tree.insert(1, "one");
  tree.insert(2, "two");
  auto it = tree.begin();
  ++it;
  ++it;
  BOOST_CHECK(it == tree.end());
}

BOOST_AUTO_TEST_CASE(test_empty_tree_iterators)
{
  AVLTree< int, std::string > tree;
  BOOST_CHECK(tree.begin() == tree.end());
  BOOST_CHECK(tree.cbegin() == tree.cend());
}

BOOST_AUTO_TEST_CASE(test_height_empty)
{
  AVLTree< int, std::string > tree;
  BOOST_CHECK_EQUAL(tree.height(), 0);
}

BOOST_AUTO_TEST_CASE(test_height_single)
{
  AVLTree< int, std::string > tree;
  tree.insert(1, "one");
  BOOST_CHECK_EQUAL(tree.height(), 1);
}

BOOST_AUTO_TEST_CASE(test_height_balanced)
{
  AVLTree< int, std::string > tree;
  tree.insert(2, "two");
  tree.insert(1, "one");
  tree.insert(3, "three");
  BOOST_CHECK_EQUAL(tree.height(), 2);
}

BOOST_AUTO_TEST_CASE(test_collect_in_range)
{
  AVLTree< int, std::string > tree;
  for (int i = 1; i <= 10; ++i)
  {
    tree.insert(i, std::to_string(i));
  }
  List< std::pair< const int, std::string > > result;
  tree.collectInRange(3, 7, result);
  BOOST_CHECK_EQUAL(result.size(), 5);
  int expected[] = {3, 4, 5, 6, 7};
  int index = 0;
  for (auto it = result.begin(); it != result.end(); ++it, ++index)
  {
    BOOST_CHECK_EQUAL((*it).first, expected[index]);
  }
}

BOOST_AUTO_TEST_CASE(test_collect_in_range_empty)
{
  AVLTree< int, std::string > tree;
  for (int i = 1; i <= 5; ++i)
  {
    tree.insert(i, std::to_string(i));
  }
  List< std::pair< const int, std::string > > result;
  tree.collectInRange(10, 20, result);
  BOOST_CHECK_EQUAL(result.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_collect_in_range_from_greater_than_to)
{
  AVLTree< int, std::string > tree;
  tree.insert(1, "one");
  List< std::pair< const int, std::string > > result;
  tree.collectInRange(5, 3, result);
  BOOST_CHECK_EQUAL(result.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_get_all)
{
  AVLTree< int, std::string > tree;
  tree.insert(3, "three");
  tree.insert(1, "one");
  tree.insert(2, "two");
  tree.insert(5, "five");
  tree.insert(4, "four");
  List< std::pair< const int, std::string > > all;
  tree.getAll(all);
  BOOST_CHECK_EQUAL(all.size(), 5);
  int expected[] = {1, 2, 3, 4, 5};
  int index = 0;
  for (auto it = all.begin(); it != all.end(); ++it, ++index)
  {
    BOOST_CHECK_EQUAL((*it).first, expected[index]);
  }
}

BOOST_AUTO_TEST_CASE(test_double_keys)
{
  AVLTree< double, int > tree;
  tree.insert(3.14, 314);
  tree.insert(2.71, 271);
  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK_EQUAL(tree.at(3.14), 314);
  BOOST_CHECK_EQUAL(tree.at(2.71), 271);
}

BOOST_AUTO_TEST_CASE(test_string_keys)
{
  AVLTree< std::string, int > tree;
  tree.insert("apple", 1);
  tree.insert("banana", 2);
  tree.insert("cherry", 3);
  BOOST_CHECK_EQUAL(tree.size(), 3);
  BOOST_CHECK_EQUAL(tree.at("apple"), 1);
  BOOST_CHECK_EQUAL(tree.at("banana"), 2);
  BOOST_CHECK_EQUAL(tree.at("cherry"), 3);
}

BOOST_AUTO_TEST_CASE(test_custom_comparator)
{
  AVLTree< int, std::string, std::greater< int > > tree;
  tree.insert(1, "one");
  tree.insert(2, "two");
  tree.insert(3, "three");
  Vector< int > keys;
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    keys.pushBack((*it).first);
  }
  BOOST_REQUIRE_EQUAL(keys.getSize(), 3);
  BOOST_CHECK_EQUAL(keys[0], 3);
  BOOST_CHECK_EQUAL(keys[1], 2);
  BOOST_CHECK_EQUAL(keys[2], 1);
}

BOOST_AUTO_TEST_SUITE_END()
