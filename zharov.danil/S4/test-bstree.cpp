#include <stdexcept>
#include <string>
#include <boost/test/unit_test.hpp>
#include <bstree.hpp>
#include <vector.hpp>

struct comp
{
  bool operator()(int f, int s) const
  {
    return f < s;
  }
};

BOOST_AUTO_TEST_SUITE(DefaultConstructorSuite)
BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  zharov::BSTree< int, std::string, comp > tree;
  BOOST_CHECK_EQUAL(tree.getSize(), 0);
  BOOST_CHECK_EQUAL(tree.height(), 0);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(CopyConstructorSuite)
BOOST_AUTO_TEST_CASE(CopyConstructor)
{
  zharov::BSTree< int, std::string, comp > original;
  original.push(1, "one");
  original.push(2, "two");
  zharov::BSTree< int, std::string, comp > copy(original);
  BOOST_CHECK_EQUAL(copy.getSize(), 2);
  BOOST_CHECK_EQUAL(copy.get(1), "one");
  BOOST_CHECK_EQUAL(copy.get(2), "two");
  original.push(3, "three");
  BOOST_CHECK_EQUAL(copy.getSize(), 2);
  BOOST_CHECK_THROW(copy.get(3), std::logic_error);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(MoveConstructorSuite)
BOOST_AUTO_TEST_CASE(MoveConstructor)
{
  zharov::BSTree< int, std::string, comp > original;
  original.push(10, "ten");
  original.push(20, "twenty");
  zharov::BSTree< int, std::string, comp > moved(std::move(original));
  BOOST_CHECK_EQUAL(moved.getSize(), 2);
  BOOST_CHECK_EQUAL(moved.get(10), "ten");
  BOOST_CHECK_EQUAL(moved.get(20), "twenty");
  BOOST_CHECK_EQUAL(original.getSize(), 0);
  BOOST_CHECK_EQUAL(original.height(), 0);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(CopyAssignmentOperatorSuite)
BOOST_AUTO_TEST_CASE(CopyAssignmentSelfAssign)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(5, "five");
  tree = tree;
  BOOST_CHECK_EQUAL(tree.getSize(), 1);
  BOOST_CHECK_EQUAL(tree.get(5), "five");
}
BOOST_AUTO_TEST_CASE(CopyAssignmentDifferentTrees)
{
  zharov::BSTree< int, std::string, comp > a, b;
  a.push(100, "hundred");
  b.push(200, "two hundred");
  b = a;
  BOOST_CHECK_EQUAL(b.getSize(), 1);
  BOOST_CHECK_EQUAL(b.get(100), "hundred");
  BOOST_CHECK_THROW(b.get(200), std::logic_error);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(MoveAssignmentOperatorSuite)
BOOST_AUTO_TEST_CASE(MoveAssignment)
{
  zharov::BSTree< int, std::string, comp > a, b;
  a.push(7, "seven");
  a.push(8, "eight");
  b = std::move(a);
  BOOST_CHECK_EQUAL(b.getSize(), 2);
  BOOST_CHECK_EQUAL(b.get(7), "seven");
  BOOST_CHECK_EQUAL(b.get(8), "eight");
  BOOST_CHECK_EQUAL(a.getSize(), 0);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(SwapSuite)
BOOST_AUTO_TEST_CASE(Swap)
{
  zharov::BSTree< int, std::string, comp > a, b;
  a.push(1, "first");
  b.push(2, "second");
  a.swap(b);
  BOOST_CHECK_EQUAL(a.getSize(), 1);
  BOOST_CHECK_EQUAL(a.get(2), "second");
  BOOST_CHECK_EQUAL(b.getSize(), 1);
  BOOST_CHECK_EQUAL(b.get(1), "first");
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(ClearSuite)
BOOST_AUTO_TEST_CASE(Clear)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(42, "answer");
  tree.push(13, "unlucky");
  tree.clear();
  BOOST_CHECK_EQUAL(tree.getSize(), 0);
  BOOST_CHECK_EQUAL(tree.height(), 0);
  BOOST_CHECK_THROW(tree.at(42), std::logic_error);
  BOOST_CHECK_THROW(tree.get(13), std::logic_error);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(PushSuite)
BOOST_AUTO_TEST_CASE(PushLvalue)
{
  zharov::BSTree< int, std::string, comp > tree;
  const int key = 10;
  const std::string value = "ten";
  tree.push(key, value);
  BOOST_CHECK_EQUAL(tree.getSize(), 1);
  BOOST_CHECK_EQUAL(tree.get(10), "ten");
}
BOOST_AUTO_TEST_CASE(PushRvalue)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(20, std::string("twenty"));
  BOOST_CHECK_EQUAL(tree.getSize(), 1);
  BOOST_CHECK_EQUAL(tree.get(20), "twenty");
}
BOOST_AUTO_TEST_CASE(PushDuplicateUpdatesValue)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(3, "old");
  tree.push(3, "new");
  BOOST_CHECK_EQUAL(tree.getSize(), 1);
  BOOST_CHECK_EQUAL(tree.get(3), "new");
}
BOOST_AUTO_TEST_CASE(PushMultipleElements)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");
  tree.push(2, "two");
  tree.push(4, "four");
  BOOST_CHECK_EQUAL(tree.getSize(), 5);
  BOOST_CHECK_EQUAL(tree.height(), 3);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(GetSuite)
BOOST_AUTO_TEST_CASE(GetExistingKey)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(15, "fifteen");
  BOOST_CHECK_EQUAL(tree.get(15), "fifteen");
}
BOOST_AUTO_TEST_CASE(GetNonExistentKeyThrows)
{
  zharov::BSTree< int, std::string, comp > tree;
  BOOST_CHECK_THROW(tree.get(99), std::logic_error);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(AtSuite)
BOOST_AUTO_TEST_CASE(AtNonConstModifies)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(50, "initial");
  tree.at(50) = "modified";
  BOOST_CHECK_EQUAL(tree.get(50), "modified");
}
BOOST_AUTO_TEST_CASE(AtConstReturnsRef)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(60, "constant");
  const auto& ctree = tree;
  BOOST_CHECK_EQUAL(ctree.at(60), "constant");
}
BOOST_AUTO_TEST_CASE(AtThrowsOnMissingKey)
{
  zharov::BSTree< int, std::string, comp > tree;
  BOOST_CHECK_THROW(tree.at(404), std::logic_error);
  const auto& ctree = tree;
  BOOST_CHECK_THROW(ctree.at(404), std::logic_error);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(HightSuite)
BOOST_AUTO_TEST_CASE(HeightFromIterator)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");
  tree.push(2, "two");
  tree.push(4, "four");
  tree.push(6, "six");
  tree.push(8, "eight");

  auto it = tree.cbegin();
  while (it != tree.cend() && it->first != 3)
  {
    ++it;
  }
  BOOST_REQUIRE(it != tree.cend());
  BOOST_CHECK_EQUAL(tree.height(it), 2);
  auto leaf = tree.cbegin();
  BOOST_CHECK_EQUAL(tree.height(leaf), 1);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(DropSuite)
BOOST_AUTO_TEST_CASE(DropLeafNode)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(8, "eight");
  tree.push(3, "three");
  std::string removed = tree.get(3);
  tree.pop(3);
  BOOST_CHECK_EQUAL(removed, "three");
  BOOST_CHECK_EQUAL(tree.getSize(), 1);
  BOOST_CHECK_THROW(tree.get(3), std::logic_error);
  BOOST_CHECK_EQUAL(tree.get(8), "eight");
}
BOOST_AUTO_TEST_CASE(DropNodeWithOneChild)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(8, "eight");
  tree.push(3, "three");
  tree.push(1, "one");
  tree.pop(3);
  BOOST_CHECK_EQUAL(tree.getSize(), 2);
  BOOST_CHECK_THROW(tree.get(3), std::logic_error);
  BOOST_CHECK_EQUAL(tree.get(1), "one");
  BOOST_CHECK_EQUAL(tree.get(8), "eight");
}
BOOST_AUTO_TEST_CASE(DropNodeWithTwoChildren)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(10, "ten");
  tree.push(5, "five");
  tree.push(15, "fifteen");
  tree.push(3, "three");
  tree.push(7, "seven");

  std::string removed = tree.get(5);
  tree.pop(5);
  BOOST_CHECK_EQUAL(removed, "five");
  BOOST_CHECK_EQUAL(tree.getSize(), 4);
  BOOST_CHECK_THROW(tree.get(5), std::logic_error);

  BOOST_CHECK_EQUAL(tree.get(7), "seven");
  BOOST_CHECK_NO_THROW(tree.get(7));
}
BOOST_AUTO_TEST_CASE(DropRoot)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(100, "hundred");
  std::string removed = tree.get(100);
  tree.pop(100);
  BOOST_CHECK_EQUAL(removed, "hundred");
  BOOST_CHECK_EQUAL(tree.getSize(), 0);
  BOOST_CHECK_EQUAL(tree.height(), 0);
}
BOOST_AUTO_TEST_CASE(DropNonExistentKeyThrows)
{
  zharov::BSTree< int, std::string, comp > tree;
  BOOST_CHECK_THROW(tree.pop(123), std::out_of_range);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(HeightSuite)
BOOST_AUTO_TEST_CASE(EmptyTreeHeight)
{
  zharov::BSTree< int, std::string, comp > tree;
  BOOST_CHECK_EQUAL(tree.height(), 0);
}
BOOST_AUTO_TEST_CASE(SingleElementHeight)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(1, "one");
  BOOST_CHECK_EQUAL(tree.height(), 1);
}
BOOST_AUTO_TEST_CASE(LinearTreeHeight)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(1, "a");
  tree.push(2, "b");
  tree.push(3, "c");
  BOOST_CHECK_EQUAL(tree.height(), 3);
}
BOOST_AUTO_TEST_CASE(BalancedTreeHeight)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(4, "root");
  tree.push(2, "L");
  tree.push(6, "R");
  tree.push(1, "LL");
  tree.push(3, "LR");
  tree.push(5, "RL");
  tree.push(7, "RR");
  BOOST_CHECK_EQUAL(tree.height(), 3);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(IteratorSuite)

BOOST_AUTO_TEST_CASE(EmptyTreeBeginEnd)
{
  zharov::BSTree< int, std::string, comp > tree;
  BOOST_CHECK(tree.begin() == tree.end());
  BOOST_CHECK(tree.cbegin() == tree.cend());
}

BOOST_AUTO_TEST_CASE(EmptyTreeCbeginCend)
{
  zharov::BSTree< int, std::string, comp > tree;
  auto it = tree.cbegin();
  auto end = tree.cend();
  BOOST_CHECK(it == end);
}

BOOST_AUTO_TEST_CASE(NonEmptyTreeBeginEnd)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");
  BOOST_CHECK(tree.begin() != tree.end());

  auto it = tree.begin();
  BOOST_CHECK_EQUAL(it->first, 3);
  BOOST_CHECK_EQUAL(it->second, "three");
}

BOOST_AUTO_TEST_CASE(Travers)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(3, "three");
  tree.push(7, "seven");
  tree.push(2, "two");

  auto it = tree.begin();
  BOOST_CHECK_EQUAL(it->first, 2);
  ++it;
  BOOST_CHECK_EQUAL(it->first, 3);
  --it;
  BOOST_CHECK_EQUAL(it->first, 2);
}

BOOST_AUTO_TEST_CASE(PostIncrement)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(10, "ten");
  tree.push(20, "twenty");

  auto it = tree.begin();
  auto old = it++;
  BOOST_CHECK_EQUAL(old->first, 10);
  BOOST_CHECK_EQUAL(it->first, 20);

  auto end = tree.end();
  auto oldEnd = end++;
  BOOST_CHECK(oldEnd == tree.end());
}

BOOST_AUTO_TEST_CASE(DereferenceAndModify)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(1, "one");
  auto it = tree.begin();
  std::pair< const int, std::string >& p = *it;
  BOOST_CHECK_EQUAL(p.first, 1);
  BOOST_CHECK_EQUAL(p.second, "one");
  p.second = "modified";
  BOOST_CHECK_EQUAL(tree.at(1), "modified");
}

BOOST_AUTO_TEST_CASE(ArrowOperator)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(1, "one");
  auto it = tree.begin();
  BOOST_CHECK_EQUAL(it->first, 1);
  BOOST_CHECK_EQUAL(it->second, "one");
}

BOOST_AUTO_TEST_CASE(EqualityComparison)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(1, "a");
  tree.push(2, "b");

  auto it1 = tree.begin();
  auto it2 = tree.begin();
  BOOST_CHECK(it1 == it2);
  ++it1;
  BOOST_CHECK(it1 != it2);
  it1 = tree.end();
  BOOST_CHECK(it1 == tree.end());
}

BOOST_AUTO_TEST_CASE(ConstIteratorReadOnly)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(1, "one");
  tree.push(2, "two");

  const auto& ctree = tree;
  auto cit = ctree.cbegin();
  BOOST_CHECK_EQUAL(cit->first, 1);
  BOOST_CHECK_EQUAL(cit->second, "one");
  ++cit;
  BOOST_CHECK_EQUAL(cit->first, 2);
}

BOOST_AUTO_TEST_CASE(IncrementEnd)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(42, "answer");
  auto it = tree.end();
  auto copy = it;
  ++it;
  BOOST_CHECK(it == copy);
  ++copy;
  BOOST_CHECK(copy == tree.end());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(RotationSuite)

BOOST_AUTO_TEST_CASE(SimpleRotateLeft)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");
  tree.push(6, "six");
  tree.push(8, "eight");

  auto it = tree.cbegin();
  while (it->first != 5)
  {
    ++it;
  }
  tree.rotateLeft(it);
  zharov::BSTree< int, std::string, comp > expected;
  expected.push(7, "seven");
  expected.push(5, "five");
  expected.push(8, "eight");
  expected.push(3, "three");
  expected.push(6, "six");

  BOOST_CHECK(tree.isStructEqual(expected));
}

BOOST_AUTO_TEST_CASE(SimpleRotateRight)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(7, "seven");
  tree.push(5, "five");
  tree.push(9, "nine");
  tree.push(3, "three");
  tree.push(6, "six");

  auto it = tree.cbegin();
  while (it != tree.cend() && it->first != 7)
  {
    ++it;
  }
  BOOST_REQUIRE(it != tree.cend());

  tree.rotateRight(it);
  zharov::BSTree< int, std::string, comp > expected;
  expected.push(5, "five");
  expected.push(3, "three");
  expected.push(7, "seven");
  expected.push(6, "six");
  expected.push(9, "nine");

  BOOST_CHECK(tree.isStructEqual(expected));
}

BOOST_AUTO_TEST_CASE(RotateLargeLeft)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(8, "eight");
  tree.push(7, "seven");

  auto it = tree.cbegin();
  while (it != tree.cend() && it->first != 5)
  {
    ++it;
  }
  BOOST_REQUIRE(it != tree.cend());

  tree.rotateLargeLeft(it);
  zharov::BSTree< int, std::string, comp > expected;
  expected.push(7, "seven");
  expected.push(5, "five");
  expected.push(8, "eight");
  expected.push(3, "three");

  BOOST_CHECK(tree.isStructEqual(expected));
}

BOOST_AUTO_TEST_CASE(RotateLargeRight)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(9, "nine");
  tree.push(6, "six");
  tree.push(12, "twelve");
  tree.push(7, "seven");

  auto it = tree.cbegin();
  while (it != tree.cend() && it->first != 9)
  {
    ++it;
  }
  BOOST_REQUIRE(it != tree.cend());

  tree.rotateLargeRight(it);
  zharov::BSTree< int, std::string, comp > expected;
  expected.push(7, "seven");
  expected.push(6, "six");
  expected.push(9, "nine");
  expected.push(12, "twelve");

  BOOST_CHECK(tree.isStructEqual(expected));
}

BOOST_AUTO_TEST_SUITE_END()
