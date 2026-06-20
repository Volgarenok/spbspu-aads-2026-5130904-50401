#define BOOST_TEST_MODULE S4

#include <boost/test/included/unit_test.hpp>

#include <stdexcept>

#include "bstree.hpp"


namespace
{
  struct IntCompare
  {
    bool operator()(int lhs, int rhs) const
    {
      return lhs < rhs;
    }
  };
}

BOOST_AUTO_TEST_CASE(defaultConstructorTest)
{
  ulanova::BSTree< int, int, IntCompare > tree;

  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.size() == 0);
  BOOST_TEST(tree.height() == 0);
}

BOOST_AUTO_TEST_CASE(pushAndGetTest)
{
  ulanova::BSTree< int, int, IntCompare > tree;

  tree.push(2, 20);
  tree.push(1, 10);
  tree.push(3, 30);

  BOOST_TEST(!tree.empty());
  BOOST_TEST(tree.size() == 3);
  BOOST_TEST(tree.get(1) == 10);
  BOOST_TEST(tree.get(2) == 20);
  BOOST_TEST(tree.get(3) == 30);
}

BOOST_AUTO_TEST_CASE(pushUpdatesExistingKeyTest)
{
  ulanova::BSTree< int, int, IntCompare > tree;

  tree.push(1, 10);
  tree.push(1, 15);

  BOOST_TEST(tree.size() == 1);
  BOOST_TEST(tree.get(1) == 15);
}

BOOST_AUTO_TEST_CASE(iteratorOrderTest)
{
  ulanova::BSTree< int, int, IntCompare > tree;

  tree.push(2, 20);
  tree.push(1, 10);
  tree.push(3, 30);

  int expected_key = 1;
  for (auto it = tree.cbegin(); it != tree.cend(); ++it)
  {
    BOOST_TEST(it->first == expected_key);
    ++expected_key;
  }

  BOOST_TEST(expected_key == 4);
}

BOOST_AUTO_TEST_CASE(dropLeafTest)
{
  ulanova::BSTree< int, int, IntCompare > tree;

  tree.push(2, 20);
  tree.push(1, 10);
  tree.push(3, 30);

  BOOST_TEST(tree.drop(1) == 10);
  BOOST_TEST(tree.size() == 2);
  BOOST_CHECK_THROW(tree.get(1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(dropNodeWithOneChildTest)
{
  ulanova::BSTree< int, int, IntCompare > tree;

  tree.push(2, 20);
  tree.push(1, 10);
  tree.push(0, 0);

  BOOST_TEST(tree.drop(1) == 10);
  BOOST_TEST(tree.size() == 2);
  BOOST_TEST(tree.get(0) == 0);
  BOOST_TEST(tree.get(2) == 20);
}

BOOST_AUTO_TEST_CASE(dropNodeWithTwoChildrenTest)
{
  ulanova::BSTree< int, int, IntCompare > tree;

  tree.push(4, 40);
  tree.push(2, 20);
  tree.push(6, 60);
  tree.push(1, 10);
  tree.push(3, 30);
  tree.push(5, 50);
  tree.push(7, 70);

  BOOST_TEST(tree.drop(4) == 40);
  BOOST_TEST(tree.size() == 6);

  int expected_key = 1;
  for (auto it = tree.cbegin(); it != tree.cend(); ++it)
  {
    if (expected_key == 4)
    {
      ++expected_key;
    }

    BOOST_TEST(it->first == expected_key);
    ++expected_key;
  }

  BOOST_TEST(expected_key == 8);
}

BOOST_AUTO_TEST_CASE(clearTest)
{
  ulanova::BSTree< int, int, IntCompare > tree;

  tree.push(1, 10);
  tree.push(2, 20);
  tree.clear();

  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.size() == 0);
  BOOST_TEST(tree.height() == 0);
}
