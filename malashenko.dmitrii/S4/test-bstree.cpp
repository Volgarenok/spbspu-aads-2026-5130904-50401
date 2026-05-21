#include <boost/test/unit_test.hpp>
#include <iostream>
#include "BStree.hpp"
namespace mal = malashenko;
using tree_t = mal::BSTree< int, char >;
BOOST_AUTO_TEST_SUITE(BSTree_constructors_and_operators)
BOOST_AUTO_TEST_CASE(common_constructor)
{
  tree_t treeEmpty = tree_t();
  BOOST_CHECK(treeEmpty.height() == 0);
}

BOOST_AUTO_TEST_CASE(copy_constructor_with_empty_tree)
{
  tree_t tree = tree_t();
  tree_t copyTree = tree_t(tree);
  BOOST_CHECK(copyTree.empty());
  BOOST_CHECK(copyTree.height() == 0);

}

BOOST_AUTO_TEST_CASE(copy_constructor_with_non_empty_tree)
{
  tree_t tree = tree_t();
  tree.push(2, 'b');
  tree.push(1, 'a');
  tree.push(3, 'c');

  tree_t copyTree = tree_t(tree);
  BOOST_CHECK(!copyTree.empty());
  BOOST_CHECK(copyTree.begin()->first == 1);
  BOOST_CHECK(copyTree.height() == 2);
}

BOOST_AUTO_TEST_CASE(move_constructor_with_empty_tree)
{
  tree_t tree = tree_t();
  tree_t copyTree(std::move(tree));
  BOOST_CHECK(copyTree.empty());
  BOOST_CHECK(copyTree.height() == 0);
  BOOST_CHECK(tree.empty());
}

BOOST_AUTO_TEST_CASE(move_constructor_with_non_empty_tree)
{
  tree_t tree = tree_t();
  tree.push(2, 'b');
  tree.push(1, 'a');
  tree.push(3, 'c');

  tree_t copyTree = tree_t(std::move(tree));
  BOOST_CHECK(!copyTree.empty());
  BOOST_CHECK(copyTree.begin()->first == 1);
  BOOST_CHECK(copyTree.height() == 2);
}


BOOST_AUTO_TEST_CASE(copy_operator_with_empty_tree)
{
  tree_t tree = tree_t();
  tree_t copyTree;
  copyTree = tree;
  BOOST_CHECK(copyTree.empty());
  BOOST_CHECK(copyTree.height() == 0);

}

BOOST_AUTO_TEST_CASE(copy_operator_with_non_empty_tree)
{
  tree_t tree = tree_t();
  tree.push(2, 'b');
  tree.push(1, 'a');
  tree.push(3, 'c');

  tree_t copyTree;
  copyTree = tree;
  BOOST_CHECK(!copyTree.empty());
  BOOST_CHECK(copyTree.begin()->first == 1);
  BOOST_CHECK(copyTree.height() == 2);
}

BOOST_AUTO_TEST_CASE(move_operator_with_empty_tree)
{
  tree_t tree = tree_t();
  tree_t copyTree;
  copyTree = std::move(tree);
  BOOST_CHECK(copyTree.empty());
  BOOST_CHECK(copyTree.height() == 0);
  BOOST_CHECK(tree.empty());
}

BOOST_AUTO_TEST_CASE(move_operator_with_non_empty_tree)
{
  tree_t tree = tree_t();
  tree.push(2, 'b');
  tree.push(1, 'a');
  tree.push(3, 'c');

  tree_t copyTree;
  copyTree = std::move(tree);
  BOOST_CHECK(!copyTree.empty());
  BOOST_CHECK(copyTree.begin()->first == 1);
  BOOST_CHECK(copyTree.height() == 2);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(BSTree_key_value_funcs)

BOOST_AUTO_TEST_CASE(push_normal_value)
{
  tree_t tree = tree_t();
  tree.push(2, 'b');
  tree.push(1, 'a');
  tree.push(3, 'c');

  BOOST_CHECK(tree.get(1) = 'a');
  BOOST_CHECK(tree.get(2) = 'b');
  BOOST_CHECK(tree.get(3) = 'c');
}

BOOST_AUTO_TEST_CASE(push_value_again)
{
  tree_t tree = tree_t();
  tree.push(1, 'a');

  BOOST_CHECK(tree.get(1) = 'a');

  tree.push(1, 'b');
  BOOST_CHECK(tree.get(1) = 'b');
}

BOOST_AUTO_TEST_SUITE_END()

