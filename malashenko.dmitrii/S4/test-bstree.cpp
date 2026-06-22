#include <boost/test/unit_test.hpp>
#include <iostream>
#include "BStree.hpp"
namespace mal = malashenko;
using tree_t = mal::BSTree< int, char >;
using iter_t = tree_t::cIter;

BOOST_AUTO_TEST_SUITE(BSTree_constructors_and_operators)
BOOST_AUTO_TEST_CASE(common_constructor)
{
  tree_t treeEmpty = tree_t();
  BOOST_CHECK_EQUAL(treeEmpty.height(), 0);
}

BOOST_AUTO_TEST_CASE(copy_constructor_with_empty_tree)
{
  tree_t tree;
  tree_t copyTree = tree_t(tree);
  BOOST_CHECK(copyTree.empty());
  BOOST_CHECK_EQUAL(copyTree.height(), 0);

}

BOOST_AUTO_TEST_CASE(copy_constructor_with_non_empty_tree)
{
  tree_t tree;
  tree.push(2, 'b');
  tree.push(1, 'a');
  tree.push(3, 'c');

  tree_t copyTree = tree_t(tree);
  BOOST_CHECK(!copyTree.empty());
  BOOST_CHECK_EQUAL(copyTree.begin()->first, 1);
  BOOST_CHECK_EQUAL(copyTree.height(), 2);
}

BOOST_AUTO_TEST_CASE(move_constructor_with_empty_tree)
{
  tree_t tree;
  tree_t copyTree(std::move(tree));
  BOOST_CHECK(copyTree.empty());
  BOOST_CHECK_EQUAL(copyTree.height(), 0);
  BOOST_CHECK(tree.empty());
}

BOOST_AUTO_TEST_CASE(move_constructor_with_non_empty_tree)
{
  tree_t tree;
  tree.push(2, 'b');
  tree.push(1, 'a');
  tree.push(3, 'c');

  tree_t copyTree = tree_t(std::move(tree));
  BOOST_CHECK(!copyTree.empty());
  BOOST_CHECK_EQUAL(copyTree.begin()->first , 1);
  BOOST_CHECK_EQUAL(copyTree.height() , 2);
}


BOOST_AUTO_TEST_CASE(copy_operator_with_empty_tree)
{
  tree_t tree;
  tree_t copyTree;
  copyTree = tree;
  BOOST_CHECK(copyTree.empty());
  BOOST_CHECK_EQUAL(copyTree.height() , 0);

}

BOOST_AUTO_TEST_CASE(copy_operator_with_non_empty_tree)
{
  tree_t tree;
  tree.push(2, 'b');
  tree.push(1, 'a');
  tree.push(3, 'c');

  tree_t copyTree;
  copyTree = tree;
  BOOST_CHECK(!copyTree.empty());
  BOOST_CHECK_EQUAL(copyTree.begin()->first , 1);
  BOOST_CHECK_EQUAL(copyTree.height() , 2);
}

BOOST_AUTO_TEST_CASE(move_operator_with_empty_tree)
{
  tree_t tree;
  tree_t copyTree;
  copyTree = std::move(tree);
  BOOST_CHECK(copyTree.empty());
  BOOST_CHECK_EQUAL(copyTree.height(), 0);
  BOOST_CHECK(tree.empty());
}

BOOST_AUTO_TEST_CASE(move_operator_with_non_empty_tree)
{
  tree_t tree;
  tree.push(2, 'b');
  tree.push(1, 'a');
  tree.push(3, 'c');

  tree_t copyTree;
  copyTree = std::move(tree);
  BOOST_CHECK(!copyTree.empty());
  BOOST_CHECK_EQUAL(copyTree.begin()->first, 1);
  BOOST_CHECK_EQUAL(copyTree.height(), 2);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(BSTree_key_value_funcs)

BOOST_AUTO_TEST_CASE(push_normal_value)
{
  tree_t tree;
  tree.push(2, 'b');
  tree.push(1, 'a');
  tree.push(3, 'c');

  BOOST_CHECK(tree.get(1) = 'a');
  BOOST_CHECK(tree.get(2) = 'b');
  BOOST_CHECK(tree.get(3) = 'c');
}

BOOST_AUTO_TEST_CASE(push_value_again)
{
  tree_t tree;
  tree.push(1, 'a');

  BOOST_CHECK_EQUAL(tree.get(1), 'a');

  tree.push(1, 'b');
  BOOST_CHECK_EQUAL(tree.get(1), 'b');
}

BOOST_AUTO_TEST_CASE(get_normal_value)
{
  tree_t tree;
  tree.push(2, 'b');
  tree.push(1, 'a');
  tree.push(3, 'c');

  BOOST_CHECK_EQUAL(tree.get(1), 'a');
  BOOST_CHECK_EQUAL(tree.get(2), 'b');
  BOOST_CHECK_EQUAL(tree.get(3), 'c');
}

BOOST_AUTO_TEST_CASE(get_unused_value)
{
  tree_t tree;

  BOOST_CHECK_THROW(tree.get(10), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(drop_from_empty_tree)
{
  tree_t tree;

  BOOST_CHECK_THROW(tree.drop(10), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(drop_single_node)
{
  tree_t tree;
  tree.push(1, 'a');

  char value = tree.drop(1);

  BOOST_CHECK_EQUAL(value, 'a');
  BOOST_CHECK(tree.empty());
}

BOOST_AUTO_TEST_CASE(drop_leaf_node)
{
  tree_t tree;

  tree.push(2, 'b');
  tree.push(1, 'a');
  tree.push(3, 'c');

  char value = tree.drop(1);

  BOOST_CHECK_EQUAL(value, 'a');

  BOOST_CHECK_EQUAL(tree.get(2), 'b');
  BOOST_CHECK_EQUAL(tree.get(3), 'c');

  BOOST_CHECK_THROW(tree.get(1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(drop_root_with_two_children)
{
  tree_t tree;

  tree.push(4, 'd');
  tree.push(2, 'b');
  tree.push(6, 'f');
  tree.push(1, 'a');
  tree.push(3, 'c');
  tree.push(5, 'e');
  tree.push(7, 'g');

  char value = tree.drop(4);

  BOOST_CHECK_EQUAL(value, 'd');

  BOOST_CHECK_THROW(tree.get(4), std::out_of_range);

  BOOST_CHECK_EQUAL(tree.get(1), 'a');
  BOOST_CHECK_EQUAL(tree.get(2), 'b');
  BOOST_CHECK_EQUAL(tree.get(3), 'c');
  BOOST_CHECK_EQUAL(tree.get(5), 'e');
  BOOST_CHECK_EQUAL(tree.get(6), 'f');
  BOOST_CHECK_EQUAL(tree.get(7), 'g');
}

BOOST_AUTO_TEST_CASE(drop_internal_node_with_two_children)
{
  tree_t tree;

  tree.push(10, 'a');
  tree.push(5, 'b');
  tree.push(15, 'c');
  tree.push(3, 'd');
  tree.push(7, 'e');

  char value = tree.drop(5);

  BOOST_CHECK_EQUAL(value, 'b');

  BOOST_CHECK_THROW(tree.get(5), std::out_of_range);

  BOOST_CHECK_EQUAL(tree.get(3), 'd');
  BOOST_CHECK_EQUAL(tree.get(7), 'e');
  BOOST_CHECK_EQUAL(tree.get(10), 'a');
  BOOST_CHECK_EQUAL(tree.get(15), 'c');
}

BOOST_AUTO_TEST_CASE(drop_all_elements)
{
  tree_t tree;

  tree.push(2, 'b');
  tree.push(3, 'c');
  tree.push(5, 'e');

  tree.drop(2);
  tree.drop(3);
  tree.drop(5);


  BOOST_CHECK(tree.empty());
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(rotation_tests)

BOOST_AUTO_TEST_CASE(rotate_left_root)
{
  tree_t tree;

  tree.push(20, 'b');
  tree.push(10, 'a');
  tree.push(30, 'c');

  BOOST_CHECK_EQUAL(tree.height(), 2);

  iter_t it = tree.cbegin();
  ++it;

  tree.rotateLeft(it);

  std::vector<int> result;

  for (iter_t i = tree.cbegin(); i != tree.cend(); ++i)
  {
    result.push_back(i->first);
  }

  std::vector<int> expected = {10, 20, 30};

  BOOST_CHECK(result == expected);
  BOOST_CHECK_EQUAL(tree.height(), 3);
}

BOOST_AUTO_TEST_CASE(rotate_right_root)
{
  tree_t tree;

  tree.push(20, 'b');
  tree.push(30, 'a');
  tree.push(10, 'c');

  BOOST_CHECK_EQUAL(tree.height(), 2);

  iter_t it = tree.cbegin();
  ++it;

  tree.rotateRight(it);

  std::vector<int> result;

  for (iter_t i = tree.cbegin(); i != tree.cend(); ++i)
  {
    result.push_back(i->first);
  }

  std::vector<int> expected = {10, 20, 30};

  BOOST_CHECK(result == expected);
  BOOST_CHECK_EQUAL(tree.height(), 3);
}

BOOST_AUTO_TEST_CASE(rotate_left_on_leaf_does_nothing)
{
  tree_t tree;

  tree.push(10, 'a');
  tree.push(20, 'b');

  iter_t it = tree.cbegin();
  ++it;

  tree.rotateLeft(it);

  std::vector<int> result;

  for (iter_t i = tree.cbegin(); i != tree.cend(); ++i)
  {
    result.push_back(i->first);
  }

  std::vector<int> expected = {10, 20};

  BOOST_CHECK(result == expected);

}

BOOST_AUTO_TEST_CASE(rotate_right_on_leaf_does_nothing)
{
  tree_t tree;

  tree.push(20, 'a');
  tree.push(10, 'b');

  iter_t it = tree.cbegin();
  BOOST_CHECK_EQUAL(tree.height(), 2);

  tree.rotateRight(it);

  std::vector<int> result;

  for (iter_t i = tree.cbegin(); i != tree.cend(); ++i)
  {
    result.push_back(i->first);
  }

  std::vector<int> expected = {10, 20};

  BOOST_CHECK(result == expected);
  BOOST_CHECK_EQUAL(tree.height(), 2);
}

BOOST_AUTO_TEST_CASE(large_left_rotation)
{
  tree_t tree;

  tree.push(30, 'a');
  tree.push(20, 'c');
  tree.push(10, 'b');

  iter_t it = tree.cbegin();
  ++it;
  ++it;

  tree.rotateLargeLeft(it);

  std::vector<int> result;

  for (iter_t i = tree.cbegin(); i != tree.cend(); ++i)
  {
    result.push_back(i->first);
  }

  std::vector<int> expected = {10, 20, 30};

  BOOST_CHECK(result == expected);
  BOOST_CHECK_EQUAL(tree.height(), 3);
}

BOOST_AUTO_TEST_CASE(large_right_rotation)
{
  tree_t tree;

  tree.push(10, 'a');
  tree.push(30, 'b');
  tree.push(20, 'c');

  iter_t it = tree.cbegin();

  tree.rotateLargeRight(it);

  std::vector<int> result;

  for (iter_t i = tree.cbegin(); i != tree.cend(); ++i)
  {
    result.push_back(i->first);
  }

  std::vector<int> expected = {10, 20, 30};

  BOOST_CHECK(result == expected);
  BOOST_CHECK_EQUAL(tree.height(), 3);
}

BOOST_AUTO_TEST_CASE(rotations_preserve_values)
{
  tree_t tree;

  tree.push(2, 'b');
  tree.push(1, 'a');
  tree.push(3, 'c');

  iter_t it = tree.cbegin();
  ++it;

  tree.rotateLeft(it);

  BOOST_CHECK_EQUAL(tree.get(1), 'a');
  BOOST_CHECK_EQUAL(tree.get(2), 'b');
  BOOST_CHECK_EQUAL(tree.get(3), 'c');
}

BOOST_AUTO_TEST_CASE(rotation_returns_new_position)
{
  tree_t tree;

  tree.push(10, 'a');
  tree.push(20, 'b');

  iter_t it = tree.cbegin();

  iter_t result = tree.rotateLeft(it);

  BOOST_CHECK_EQUAL(result->first, 20);
}

BOOST_AUTO_TEST_CASE(rotations_do_not_break_iteration)
{
  tree_t tree;

  for (int i = 1; i <= 10; ++i)
  {
    tree.push(i, 'a');
  }

  iter_t it = tree.cbegin();
  ++it;
  ++it;
  ++it;

  tree.rotateLeft(it);

  std::vector<int> result;

  for (iter_t iter = tree.cbegin(); iter != tree.cend(); ++iter)
  {
    result.push_back(iter->first);
  }

  std::vector<int> expected = {1,2,3,4,5,6,7,8,9,10};

  BOOST_CHECK(result == expected);
}

BOOST_AUTO_TEST_SUITE_END()
