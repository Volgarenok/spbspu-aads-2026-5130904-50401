#include <boost/test/unit_test.hpp>
#include <string>
#include "bstree.hpp"

using tree_t = kondrat::BSTree< int, std::string >;

BOOST_AUTO_TEST_SUITE(BSTreeTests)

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  tree_t tree;

  BOOST_CHECK(tree.empty());
  BOOST_CHECK(tree.size() == 0);
  BOOST_CHECK(tree.begin() == tree.end());
}

BOOST_AUTO_TEST_CASE(PushGetHas)
{
  tree_t tree;

  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");

  BOOST_CHECK(!tree.empty());
  BOOST_CHECK(tree.size() == 3);

  BOOST_CHECK(tree.contains(1));
  BOOST_CHECK(tree.contains(2));
  BOOST_CHECK(tree.contains(3));
  BOOST_CHECK(!tree.contains(4));

  BOOST_CHECK(tree.at(1) == "one");
  BOOST_CHECK(tree.at(2) == "two");
  BOOST_CHECK(tree.at(3) == "three");
}

BOOST_AUTO_TEST_CASE(PushUpdatesExistingKey)
{
  tree_t tree;

  tree.push(1, "one");
  tree.push(1, "new-one");

  BOOST_CHECK(tree.size() == 1);
  BOOST_CHECK(tree.at(1) == "new-one");
}

BOOST_AUTO_TEST_CASE(RvaluePushAndSubscript)
{
  tree_t tree;
  std::string value = "one";

  tree.push(1, std::move(value));
  tree[2] = "two";

  BOOST_CHECK(tree.size() == 2);
  BOOST_CHECK(tree.at(1) == "one");
  BOOST_CHECK(tree.at(2) == "two");
}

BOOST_AUTO_TEST_CASE(AtThrows)
{
  tree_t tree;

  tree.push(1, "one");

  BOOST_CHECK_THROW(tree.at(2), std::logic_error);
}

BOOST_AUTO_TEST_CASE(IteratorOrder)
{
  tree_t tree;

  tree.push(5, "five");
  tree.push(2, "two");
  tree.push(7, "seven");
  tree.push(1, "one");
  tree.push(3, "three");

  int keys[5] = {1, 2, 3, 5, 7};
  size_t i = 0;

  for (tree_t::iterator it = tree.begin(); it != tree.end(); ++it)
  {
    BOOST_CHECK(it->first == keys[i]);
    ++i;
  }

  BOOST_CHECK(i == 5);
}

BOOST_AUTO_TEST_CASE(ConstIteratorOrder)
{
  tree_t tree;

  tree.push(3, "three");
  tree.push(1, "one");
  tree.push(2, "two");

  const tree_t & ctree = tree;

  int keys[3] = {1, 2, 3};
  size_t i = 0;

  for (tree_t::const_iterator it = ctree.begin(); it != ctree.end(); ++it)
  {
    BOOST_CHECK(it->first == keys[i]);
    ++i;
  }

  BOOST_CHECK(i == 3);
}

BOOST_AUTO_TEST_CASE(EraseLeaf)
{
  tree_t tree;

  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");

  BOOST_CHECK(tree.erase(1) == 1);
  BOOST_CHECK(tree.size() == 2);
  BOOST_CHECK(!tree.contains(1));
  BOOST_CHECK(tree.contains(2));
  BOOST_CHECK(tree.contains(3));
}

BOOST_AUTO_TEST_CASE(EraseNodeWithOneChild)
{
  tree_t tree;

  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(2, "two");

  BOOST_CHECK(tree.erase(3) == 1);
  BOOST_CHECK(tree.size() == 2);
  BOOST_CHECK(!tree.contains(3));
  BOOST_CHECK(tree.contains(2));
  BOOST_CHECK(tree.contains(5));
}

BOOST_AUTO_TEST_CASE(EraseNodeWithTwoChildren)
{
  tree_t tree;

  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");
  tree.push(6, "six");
  tree.push(8, "eight");

  BOOST_CHECK(tree.erase(7) == 1);
  BOOST_CHECK(tree.size() == 4);
  BOOST_CHECK(!tree.contains(7));
  BOOST_CHECK(tree.contains(5));
  BOOST_CHECK(tree.contains(3));
  BOOST_CHECK(tree.contains(6));
  BOOST_CHECK(tree.contains(8));
}

BOOST_AUTO_TEST_CASE(EraseMissing)
{
  tree_t tree;

  tree.push(1, "one");

  BOOST_CHECK(tree.erase(2) == 0);
  BOOST_CHECK(tree.size() == 1);
}

BOOST_AUTO_TEST_CASE(Clear)
{
  tree_t tree;

  tree.push(1, "one");
  tree.push(2, "two");

  tree.clear();

  BOOST_CHECK(tree.empty());
  BOOST_CHECK(tree.size() == 0);
  BOOST_CHECK(tree.begin() == tree.end());
}

BOOST_AUTO_TEST_CASE(Height)
{
  tree_t tree;

  BOOST_CHECK(tree.height() == 0);

  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(2, "two");
  tree.push(1, "one");

  BOOST_CHECK(tree.height() == 4);
}

BOOST_AUTO_TEST_CASE(RotateLeft)
{
  tree_t tree;

  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(3, "three");

  const tree_t & ctree = tree;
  tree_t::const_iterator it = ctree.begin();

  tree.rotateLeft(it);

  BOOST_CHECK(tree.begin()->first == 1);
  BOOST_CHECK(tree.height() == 2);
}

BOOST_AUTO_TEST_CASE(RotateRight)
{
  tree_t tree;

  tree.push(3, "three");
  tree.push(2, "two");
  tree.push(1, "one");

  const tree_t & ctree = tree;
  tree_t::const_iterator it = ctree.begin();
  ++it;
  ++it;

  tree.rotateRight(it);

  BOOST_CHECK(tree.begin()->first == 1);
  BOOST_CHECK(tree.height() == 2);
}

BOOST_AUTO_TEST_SUITE_END()
