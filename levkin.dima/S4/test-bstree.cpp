#include <boost/test/unit_test.hpp>
#include "bstree.hpp"
#include <string>
using namespace levkin;
BOOST_AUTO_TEST_CASE(bst_constructor_and_has_test)
{
  BSTree< std::string, int > tree;
  BOOST_CHECK_EQUAL(tree.has("key"), false);
  BOOST_CHECK_EQUAL(tree.height(), 0);
}
BOOST_AUTO_TEST_CASE(bst_push_and_has_test)
{
  BSTree< std::string, int > tree;
  tree.push("apple", 100);
  tree.push("banana", 200);
  BOOST_CHECK(tree.has("apple"));
  BOOST_CHECK(tree.has("banana"));
  BOOST_CHECK(!tree.has("orange"));
  BOOST_CHECK_EQUAL(tree.get("apple"), 100);
}
BOOST_AUTO_TEST_CASE(bst_update_value_test)
{
  BSTree< std::string, int > tree;
  tree.push("key1", 10);
  tree.push("key1", 20);
  BOOST_CHECK(tree.has("key1"));
  BOOST_CHECK_EQUAL(tree.get("key1"), 20);
  BOOST_CHECK_EQUAL(tree.drop("key1"), 20);
  BOOST_CHECK(!tree.has("key1"));
}
BOOST_AUTO_TEST_CASE(bst_drop_exception_test)
{
  BSTree< std::string, int > tree;
  BOOST_CHECK_THROW(tree.drop("unknown"), std::out_of_range);
}
BOOST_AUTO_TEST_CASE(bst_rotation_and_height_test)
{
  BSTree< int, std::string > tree;
  tree.push(10, "ten");
  tree.push(20, "twenty");
  tree.push(30, "thirty");
  BOOST_CHECK_EQUAL(tree.height(), 3);
  auto it = tree.cbegin();
  ++it;
  BOOST_CHECK_EQUAL(it->key, 20);
  auto new_root = tree.rotateLeft(it);
  BOOST_CHECK_EQUAL(new_root->key, 20);
  BOOST_CHECK_EQUAL(tree.height(), 2);
}
