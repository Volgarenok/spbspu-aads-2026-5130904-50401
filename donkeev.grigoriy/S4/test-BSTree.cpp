#include <boost/test/unit_test.hpp>
#include <string>
#include "BSTree.hpp"


BOOST_AUTO_TEST_SUITE(testingTree)

BOOST_AUTO_TEST_CASE(defaultConstructor)
{
  donkeev::BSTree< int, std::string, donkeev::Comp< int > > tree;
  BOOST_CHECK(tree.empty());
}

BOOST_AUTO_TEST_CASE(copyConstructor)
{
  donkeev::BSTree< int, std::string, donkeev::Comp< int > > tree;
  tree.insert(1, "a");
  donkeev::BSTree< int, std::string, donkeev::Comp< int > > other(tree);
  BOOST_CHECK(other.size() == 1);
}

BOOST_AUTO_TEST_CASE(movementConstructor)
{
  donkeev::BSTree< int, std::string, donkeev::Comp< int > > tree;
  tree.insert(1, "a");
  donkeev::BSTree< int, std::string, donkeev::Comp< int > > other(std::move(tree));
  BOOST_CHECK(other.size() == 1);
  BOOST_CHECK(tree.empty());
}

BOOST_AUTO_TEST_CASE(userConstructor)
{
  donkeev::BSTree< int, std::string, donkeev::Comp< int > > tree(1, "a");
  BOOST_CHECK(tree.size() == 1);
  BOOST_CHECK(tree[1] == "a");
}

BOOST_AUTO_TEST_CASE(movementUserConstructor)
{
  std::string str("a");
  donkeev::BSTree< int, std::string, donkeev::Comp< int > > tree(1, std::move(str));
  BOOST_CHECK(tree.size() == 1);
  BOOST_CHECK(tree[1] == "a");
}

BOOST_AUTO_TEST_CASE(destructor)
{
  donkeev::BSTree< int, std::string, donkeev::Comp< int > > tree(1, "a");
  BOOST_CHECK(tree.size() == 1);
  BOOST_CHECK(tree[1] == "a");

  tree.clear();
  BOOST_CHECK(tree.empty());
}

BOOST_AUTO_TEST_CASE(equalOperator)
{
  donkeev::BSTree< int, std::string, donkeev::Comp< int > > tree(1, "a");
  donkeev::BSTree< int, std::string, donkeev::Comp< int > > other(2, "b");
  other = tree;
  BOOST_CHECK(other.size() == 1);
  BOOST_CHECK(other[1] == "a");
}

BOOST_AUTO_TEST_CASE(movementEqualOperator)
{
  donkeev::BSTree< int, std::string, donkeev::Comp< int > > tree(1, "a");
  donkeev::BSTree< int, std::string, donkeev::Comp< int > > other(2, "b");
  other = std::move(tree);
  BOOST_CHECK(other.size() == 1);
  BOOST_CHECK(other[1] == "a");
  BOOST_CHECK(tree.empty());
}

BOOST_AUTO_TEST_CASE(beginMetod)
{
  donkeev::BSTree< int, std::string, donkeev::Comp< int > > tree(1, "a");
  tree.insert(2, "b");
  donkeev::BSTree< int, std::string, donkeev::Comp< int > >::iterator begin = tree.begin();
  BOOST_CHECK((*begin).second == "a");
}

BOOST_AUTO_TEST_CASE(endMetod)
{
  donkeev::BSTree< int, std::string, donkeev::Comp< int > > tree(1, "a");
  tree.insert(2, "b");
  donkeev::BSTree< int, std::string, donkeev::Comp< int > >::iterator end = tree.end();
  BOOST_CHECK(end == nullptr);
}

BOOST_AUTO_TEST_CASE(constBeginMetod)
{
  const donkeev::BSTree< int, std::string, donkeev::Comp< int > > tree(1, "a");
  donkeev::BSTree< int, std::string, donkeev::Comp< int > >::constIterator begin = tree.begin();
  BOOST_CHECK((*begin).second == "a");
}

BOOST_AUTO_TEST_CASE(constEndMetod)
{
  const donkeev::BSTree< int, std::string, donkeev::Comp< int > > tree(1, "a");
  donkeev::BSTree< int, std::string, donkeev::Comp< int > >::constIterator end = tree.end();
  BOOST_CHECK(end == nullptr);
}

BOOST_AUTO_TEST_CASE(rotateLeft)
{
  donkeev::BSTree<int, std::string, donkeev::Comp<int> > tree;
  tree.insert(10, "10");
  tree.insert(20, "20");
  tree.insert(30, "30");
  tree.insert(35, "35");

  BOOST_CHECK_EQUAL(tree.height(), 4);

  donkeev::BSTree<int, std::string, donkeev::Comp<int> >::iterator it = tree.find(20);
  tree.rotateLeft(it);

  BOOST_CHECK_EQUAL(tree.height(), 3);

  int expectedKeys[] = {10, 20, 30, 35};
  size_t i = 0;
  for (auto tit = tree.begin(); tit != tree.end(); ++tit, ++i)
  {
    BOOST_CHECK_EQUAL((*tit).first, expectedKeys[i]);
  }
  BOOST_CHECK_EQUAL(i, 4);
}

BOOST_AUTO_TEST_CASE(rotateRight)
{
  donkeev::BSTree<int, std::string, donkeev::Comp<int> > tree;
  tree.insert(35, "35");
  tree.insert(30, "30");
  tree.insert(20, "20");
  tree.insert(10, "10");

  BOOST_CHECK_EQUAL(tree.height(), 4);

  donkeev::BSTree<int, std::string, donkeev::Comp<int> >::iterator it = tree.find(30);
  tree.rotateRight(it);

  BOOST_CHECK_EQUAL(tree.height(), 3);

  int expectedKeys[] = {10, 20, 30, 35};
  size_t i = 0;
  for (auto tit = tree.begin(); tit != tree.end(); ++tit, ++i)
  {
    BOOST_CHECK_EQUAL((*tit).first, expectedKeys[i]);
  }
  BOOST_CHECK_EQUAL(i, 4);
}

BOOST_AUTO_TEST_CASE(largeRotateLeft)
{
    donkeev::BSTree<int, std::string, donkeev::Comp<int> > tree;
    tree.insert(10, "10");
    tree.insert(30, "30");
    tree.insert(20, "20");
    tree.insert(15, "15");
    tree.insert(25, "25");


    BOOST_CHECK_EQUAL(tree.height(), 4);

    auto it = tree.find(10);
    tree.largeRotateLeft(it);



    BOOST_CHECK_EQUAL(tree.height(), 3);

    int expectedKeys[] = {10, 15, 20, 25, 30};
    size_t i = 0;
    for (auto tit = tree.begin(); tit != tree.end(); ++tit, ++i)
    {
      BOOST_CHECK_EQUAL((*tit).first, expectedKeys[i]);
    }
    BOOST_CHECK_EQUAL(i, 5);
}

BOOST_AUTO_TEST_CASE(largeRotateRight)
{
    donkeev::BSTree<int, std::string, donkeev::Comp<int> > tree;
    tree.insert(30, "30");
    tree.insert(10, "10");
    tree.insert(20, "20");
    tree.insert(15, "15");
    tree.insert(25, "25");

    BOOST_CHECK_EQUAL(tree.height(), 4);

    auto it = tree.find(30);
    tree.largeRotateRight(it);

    BOOST_CHECK_EQUAL(tree.height(), 3);

    int expectedKeys[] = {10, 15, 20, 25, 30};
    size_t i = 0;
    for (auto tit = tree.begin(); tit != tree.end(); ++tit, ++i)
    {
      BOOST_CHECK_EQUAL((*tit).first, expectedKeys[i]);
    }
    BOOST_CHECK_EQUAL(i, 5);
}

BOOST_AUTO_TEST_CASE(movementPush)
{
  std::string str("a");
  donkeev::BSTree< int, std::string, donkeev::Comp< int > > tree;
  tree.insert(1, std::move(str));
  BOOST_CHECK((*tree.begin()).second == "a");
}

BOOST_AUTO_TEST_CASE(getMethod)
{
  donkeev::BSTree< int, std::string, donkeev::Comp< int > > tree(1, "a");
  BOOST_CHECK(tree[1] == "a");
}

BOOST_AUTO_TEST_CASE(findMethod)
{
  donkeev::BSTree< int, std::string, donkeev::Comp< int > > tree(1, "a");
  BOOST_CHECK((*tree.find(1)).second == "a");
}

BOOST_AUTO_TEST_CASE(dropMethod)
{
  donkeev::BSTree< int, std::string, donkeev::Comp< int > > tree(1, "a");
  tree.erase(1);
  BOOST_CHECK(tree.empty());
}

BOOST_AUTO_TEST_CASE(clearMethod)
{
  donkeev::BSTree< int, std::string, donkeev::Comp< int > > tree(1, "a");
  tree.insert(2, "b");
  tree.clear();
  BOOST_CHECK(tree.empty());
}

BOOST_AUTO_TEST_CASE(swapMethod)
{
  donkeev::BSTree< int, std::string, donkeev::Comp< int > > tree(1, "a");
  donkeev::BSTree< int, std::string, donkeev::Comp< int > > other(2, "b");
  tree.swap(other);
  BOOST_CHECK((*tree.begin()).second == "b");
  BOOST_CHECK((*other.begin()).second == "a");
}

BOOST_AUTO_TEST_SUITE_END()
