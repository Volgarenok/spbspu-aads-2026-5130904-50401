#define BOOST_TEST_MODULE BSTreeTests
#include <boost/test/included/unit_test.hpp>

#include <stdexcept>
#include <string>
#include "bstree.hpp"

BOOST_AUTO_TEST_SUITE(BSTreeTests)

BOOST_AUTO_TEST_CASE(push_get_update)
{
  studilova::BSTree< int, std::string > tree;

  BOOST_TEST(tree.empty());

  tree.push(10, "ten");
  tree.push(5, "five");
  tree.push(15, "fifteen");

  BOOST_TEST(!tree.empty());
  BOOST_TEST(tree.get(10) == "ten");
  BOOST_TEST(tree.get(5) == "five");
  BOOST_TEST(tree.get(15) == "fifteen");

  tree.push(10, "TEN");
  BOOST_TEST(tree.get(10) == "TEN");
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  studilova::BSTree< int, std::string > tree;
  tree.push(10, "ten");
  tree.push(5, "five");
  tree.push(15, "fifteen");

  studilova::BSTree< int, std::string > copy(tree);

  BOOST_TEST(copy.get(10) == "ten");
  BOOST_TEST(copy.get(5) == "five");
  BOOST_TEST(copy.get(15) == "fifteen");

  copy.push(10, "changed");

  BOOST_TEST(copy.get(10) == "changed");
  BOOST_TEST(tree.get(10) == "ten");
}

BOOST_AUTO_TEST_CASE(assignment_operator)
{
  studilova::BSTree< int, std::string > first;
  first.push(1, "one");
  first.push(2, "two");

  studilova::BSTree< int, std::string > second;
  second.push(10, "ten");

  second = first;

  BOOST_TEST(second.get(1) == "one");
  BOOST_TEST(second.get(2) == "two");
  BOOST_CHECK_THROW(second.get(10), std::out_of_range);
}
