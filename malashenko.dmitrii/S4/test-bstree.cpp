#include <boost/test/unit_test.hpp>
#include <iostream>
#include "BStree.hpp"
namespace mal = malashenko;
using tree_t = mal::BSTree< int, char >;
BOOST_AUTO_TEST_SUITE(BSTree_constructors)
BOOST_AUTO_TEST_CASE(common_constructor)
{
  tree_t treeEmpty = tree_t();
  BOOST_CHECK(treeEmpty.height() == 0);
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  tree_t tree = tree_t();
  tree.push(2, 'b');
  tree.push(1, 'a');
  tree.push(3, 'c');

  tree_t copyTree = tree_t(tree);

  BOOST_CHECK(tree.height() == 2);
}


BOOST_AUTO_TEST_SUITE_END()
