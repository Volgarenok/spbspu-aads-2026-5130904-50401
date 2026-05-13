#define BOOST_TEST_MODULE BSTreeTests
#include <boost/test/unit_test.hpp>
#include <string>
#include <stdexcept>
#include <utility>
#include "bstree.hpp"
#include "../common/vector.hpp"

using namespace sedov;

BOOST_AUTO_TEST_SUITE(BSTreeTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  BSTree< int, std::string > tree;
  BOOST_CHECK(tree.empty());
  BOOST_CHECK_EQUAL(tree.size(), 0);
  BOOST_CHECK(tree.begin() == tree.end());
}

BOOST_AUTO_TEST_CASE(test_push_and_size)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  BOOST_CHECK_EQUAL(tree.size(), 1);
  BOOST_CHECK(!tree.empty());

  tree.push(2, "two");
  tree.push(3, "three");
  BOOST_CHECK_EQUAL(tree.size(), 3);
}

BOOST_AUTO_TEST_CASE(test_push_update_existing)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(1, "ONE");

  BOOST_CHECK_EQUAL(tree.size(), 1);
  BOOST_CHECK_EQUAL(tree.at(1), "ONE");
}

BOOST_AUTO_TEST_CASE(test_push_rvalue)
{
  BSTree< int, std::string > tree;
  std::string val = "rvalue";
  tree.push(1, std::move(val));
  BOOST_CHECK_EQUAL(tree.at(1), "rvalue");
}

BOOST_AUTO_TEST_CASE(test_at_const)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");

  BOOST_CHECK_EQUAL(tree.at(1), "one");
  BOOST_CHECK_EQUAL(tree.at(2), "two");
}

BOOST_AUTO_TEST_CASE(test_at_non_const)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.at(1) = "ONE";
  BOOST_CHECK_EQUAL(tree.at(1), "ONE");
}

BOOST_AUTO_TEST_CASE(test_at_not_found)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");

  BOOST_CHECK_THROW(tree.at(2), std::out_of_range);
  const BSTree< int, std::string > & ctree = tree;
  BOOST_CHECK_THROW(ctree.at(2), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_drop)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(3, "three");

  std::string val = tree.drop(2);
  BOOST_CHECK_EQUAL(val, "two");
  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK_EQUAL(tree.at(1), "one");
  BOOST_CHECK_EQUAL(tree.at(3), "three");
  BOOST_CHECK_THROW(tree.at(2), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_drop_root_with_two_children)
{
  BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");

  tree.drop(2);
  BOOST_CHECK_EQUAL(tree.size(), 2);

  bool hasOne = false;
  bool hasThree = false;
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    if ((*it).first == 1)
    {
      hasOne = true;
    }
    if ((*it).first == 3)
    {
      hasThree = true;
    }
  }
  BOOST_CHECK(hasOne && hasThree);
}

BOOST_AUTO_TEST_CASE(test_drop_leaf)
{
  BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");

  tree.drop(1);
  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK_THROW(tree.at(1), std::out_of_range);
  BOOST_CHECK_EQUAL(tree.at(2), "two");
  BOOST_CHECK_EQUAL(tree.at(3), "three");
}

BOOST_AUTO_TEST_CASE(test_drop_nonexistent)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");

  BOOST_CHECK_THROW(tree.drop(999), std::out_of_range);
  BOOST_CHECK_EQUAL(tree.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_iterator_traversal)
{
  BSTree< int, std::string > tree;
  tree.push(3, "three");
  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(4, "four");

  sedov::Vector< std::pair< const int, std::string > > result;
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
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");

  const BSTree< int, std::string > & ctree = tree;
  BOOST_CHECK(ctree.cbegin() != ctree.cend());

  sedov::Vector< int > keys;
  for (auto it = ctree.cbegin(); it != ctree.cend(); ++it)
  {
    keys.pushBack((*it).first);
  }
  BOOST_CHECK_EQUAL(keys.getSize(), 2);
  BOOST_CHECK_EQUAL(keys[0], 1);
  BOOST_CHECK_EQUAL(keys[1], 2);
}

BOOST_AUTO_TEST_CASE(test_iterator_equality)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");

  auto it1 = tree.begin();
  auto it2 = tree.begin();
  auto it3 = tree.end();

  BOOST_CHECK(it1 == it2);
  BOOST_CHECK(it1 != it3);
  BOOST_CHECK(tree.begin() != tree.end());
}

BOOST_AUTO_TEST_CASE(test_end_iterator)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");

  auto it = tree.begin();
  ++it;
  ++it;
  BOOST_CHECK(it == tree.end());
}

BOOST_AUTO_TEST_CASE(test_reverse_traversal_with_decrement)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(3, "three");
  tree.push(4, "four");

  sedov::Vector< int > keys;
  auto it = tree.end();
  --it;
  while (true)
  {
    keys.pushBack((*it).first);
    if (it == tree.begin())
    {
      break;
    }
    --it;
  }

  BOOST_REQUIRE_EQUAL(keys.getSize(), 4);
  BOOST_CHECK_EQUAL(keys[0], 4);
  BOOST_CHECK_EQUAL(keys[1], 3);
  BOOST_CHECK_EQUAL(keys[2], 2);
  BOOST_CHECK_EQUAL(keys[3], 1);
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  BSTree< int, std::string > tree1;
  tree1.push(1, "one");
  tree1.push(2, "two");
  tree1.push(3, "three");

  BSTree< int, std::string > tree2(tree1);

  BOOST_CHECK_EQUAL(tree2.size(), 3);
  BOOST_CHECK_EQUAL(tree2.at(1), "one");
  BOOST_CHECK_EQUAL(tree2.at(2), "two");
  BOOST_CHECK_EQUAL(tree2.at(3), "three");

  tree2.at(1) = "ONE";
  BOOST_CHECK_EQUAL(tree1.at(1), "one");
  BOOST_CHECK_EQUAL(tree2.at(1), "ONE");
}

BOOST_AUTO_TEST_CASE(test_copy_assignment)
{
  BSTree< int, std::string > tree1;
  tree1.push(1, "one");

  BSTree< int, std::string > tree2;
  tree2.push(99, "ninety-nine");

  tree2 = tree1;

  BOOST_CHECK_EQUAL(tree2.size(), 1);
  BOOST_CHECK_EQUAL(tree2.at(1), "one");
  BOOST_CHECK_THROW(tree2.at(99), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  BSTree< int, std::string > tree1;
  tree1.push(1, "one");
  tree1.push(2, "two");

  BSTree< int, std::string > tree2(std::move(tree1));

  BOOST_CHECK_EQUAL(tree2.size(), 2);
  BOOST_CHECK_EQUAL(tree2.at(1), "one");
  BOOST_CHECK_EQUAL(tree2.at(2), "two");
  BOOST_CHECK(tree1.empty());
}

BOOST_AUTO_TEST_CASE(test_move_assignment)
{
  BSTree< int, std::string > tree1;
  tree1.push(1, "one");

  BSTree< int, std::string > tree2;
  tree2.push(99, "ninety-nine");

  tree2 = std::move(tree1);

  BOOST_CHECK_EQUAL(tree2.size(), 1);
  BOOST_CHECK_EQUAL(tree2.at(1), "one");
  BOOST_CHECK(tree1.empty());
}

BOOST_AUTO_TEST_CASE(test_swap)
{
  BSTree< int, std::string > tree1;
  tree1.push(1, "one");

  BSTree< int, std::string > tree2;
  tree2.push(2, "two");
  tree2.push(3, "three");

  tree1.swap(tree2);

  BOOST_CHECK_EQUAL(tree1.size(), 2);
  BOOST_CHECK_EQUAL(tree1.at(2), "two");
  BOOST_CHECK_EQUAL(tree2.size(), 1);
  BOOST_CHECK_EQUAL(tree2.at(1), "one");
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(3, "three");

  tree.clear();
  BOOST_CHECK(tree.empty());
  BOOST_CHECK_EQUAL(tree.size(), 0);
  BOOST_CHECK_THROW(tree.at(1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_height_empty)
{
  BSTree< int, std::string > tree;
  BOOST_CHECK_EQUAL(tree.height(), 0);
}

BOOST_AUTO_TEST_CASE(test_height_single)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  BOOST_CHECK_EQUAL(tree.height(), 1);
}

BOOST_AUTO_TEST_CASE(test_height_balanced)
{
  BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");
  BOOST_CHECK_EQUAL(tree.height(), 2);
}

BOOST_AUTO_TEST_CASE(test_height_degenerate)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(3, "three");
  tree.push(4, "four");
  BOOST_CHECK_EQUAL(tree.height(), 4);
}

BOOST_AUTO_TEST_CASE(test_height_iterator)
{
  BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");

  auto it = tree.cbegin();
  ++it;
  BOOST_CHECK_EQUAL(tree.height(it), 2);

  ++it;
  BOOST_CHECK_EQUAL(tree.height(it), 1);
}

BOOST_AUTO_TEST_CASE(test_rotate_left)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");

  auto it = tree.cbegin();
  ++it;
  auto newIt = tree.rotateLeft(it);

  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK_EQUAL((*newIt).first, 2);
  BOOST_CHECK_EQUAL(tree.at(1), "one");
  BOOST_CHECK_EQUAL(tree.at(2), "two");
}

BOOST_AUTO_TEST_CASE(test_rotate_right)
{
  BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");

  auto it = tree.cbegin();
  ++it;
  auto newIt = tree.rotateRight(it);

  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK_EQUAL((*newIt).first, 1);
  BOOST_CHECK_EQUAL(tree.at(1), "one");
  BOOST_CHECK_EQUAL(tree.at(2), "two");
}

BOOST_AUTO_TEST_CASE(test_rotate_large_left)
{
  BSTree< int, std::string > tree;
  tree.push(3, "three");
  tree.push(1, "one");
  tree.push(2, "two");

  auto it = tree.cbegin();
  ++it;
  ++it;
  tree.rotateLargeLeft(it);

  BOOST_CHECK_EQUAL(tree.size(), 3);
  BOOST_CHECK_EQUAL(tree.at(1), "one");
  BOOST_CHECK_EQUAL(tree.at(2), "two");
  BOOST_CHECK_EQUAL(tree.at(3), "three");
}

BOOST_AUTO_TEST_CASE(test_rotate_large_right)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(3, "three");
  tree.push(2, "two");

  auto it = tree.cbegin();
  tree.rotateLargeRight(it);

  BOOST_CHECK_EQUAL(tree.size(), 3);
  BOOST_CHECK_EQUAL(tree.at(1), "one");
  BOOST_CHECK_EQUAL(tree.at(2), "two");
  BOOST_CHECK_EQUAL(tree.at(3), "three");
}

BOOST_AUTO_TEST_CASE(test_empty_tree_operations)
{
  BSTree< int, std::string > tree;
  BOOST_CHECK(tree.begin() == tree.end());
  BOOST_CHECK(tree.cbegin() == tree.cend());
  BOOST_CHECK_EQUAL(tree.height(), 0);
}

BOOST_AUTO_TEST_CASE(test_different_key_types)
{
  BSTree< double, int > tree;
  tree.push(3.14, 314);
  tree.push(2.71, 271);

  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK_EQUAL(tree.at(3.14), 314);
  BOOST_CHECK_EQUAL(tree.at(2.71), 271);
}

BOOST_AUTO_TEST_CASE(test_string_keys)
{
  BSTree< std::string, int > tree;
  tree.push("apple", 1);
  tree.push("banana", 2);

  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK_EQUAL(tree.at("apple"), 1);
  BOOST_CHECK_EQUAL(tree.at("banana"), 2);
}

BOOST_AUTO_TEST_CASE(test_custom_comparator)
{
  BSTree< int, std::string, std::greater< int > > tree;
  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(3, "three");

  sedov::Vector<int> keys;
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
