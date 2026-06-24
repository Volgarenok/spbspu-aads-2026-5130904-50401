#include <string>
#include <stdexcept>

#include <boost/test/unit_test.hpp>

#include "AVLTree.hpp"

BOOST_AUTO_TEST_SUITE(AVLTreeTests)

BOOST_AUTO_TEST_CASE(defaultConstructorMakesEmptyTree)
{
  burukov::AVLTree< int, std::string > t;
  BOOST_CHECK(t.empty());
  BOOST_CHECK_EQUAL(t.size(), 0);
}

BOOST_AUTO_TEST_CASE(pushAddsNewElements)
{
  burukov::AVLTree< int, std::string > t;
  t.push(5, "five");
  t.push(3, "three");
  t.push(7, "seven");
  BOOST_CHECK_EQUAL(t.size(), 3);
  BOOST_CHECK_EQUAL(t.at(5), "five");
  BOOST_CHECK_EQUAL(t.at(3), "three");
  BOOST_CHECK_EQUAL(t.at(7), "seven");
}

BOOST_AUTO_TEST_CASE(pushUpdatesExistingKey)
{
  burukov::AVLTree< int, std::string > t;
  t.push(42, "answer");
  t.push(42, "ANSWER");
  BOOST_CHECK_EQUAL(t.size(), 1);
  BOOST_CHECK_EQUAL(t.at(42), "ANSWER");
}

BOOST_AUTO_TEST_CASE(atThrowsOnMissingKey)
{
  burukov::AVLTree< int, std::string > t;
  t.push(1, "one");
  BOOST_CHECK_THROW(t.at(2), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(hasKeyReturnsTrueForExistingKey)
{
  burukov::AVLTree< int, std::string > t;
  t.push(1, "one");
  BOOST_CHECK(t.hasKey(1));
}

BOOST_AUTO_TEST_CASE(hasKeyReturnsFalseForMissingKey)
{
  burukov::AVLTree< int, std::string > t;
  BOOST_CHECK(!t.hasKey(42));
}

BOOST_AUTO_TEST_CASE(dropRemovesNode)
{
  burukov::AVLTree< int, std::string > t;
  t.push(2, "two");
  t.push(1, "one");
  t.push(3, "three");
  std::string val = t.drop(1);
  BOOST_CHECK_EQUAL(val, "one");
  BOOST_CHECK_EQUAL(t.size(), 2);
  BOOST_CHECK_THROW(t.at(1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(dropRemovesNodeWithTwoChildren)
{
  burukov::AVLTree< int, std::string > t;
  t.push(10, "ten");
  t.push(5, "five");
  t.push(15, "fifteen");
  t.push(12, "twelve");
  t.push(18, "eighteen");
  t.drop(10);
  BOOST_CHECK_EQUAL(t.size(), 4);
  BOOST_CHECK(!t.hasKey(10));
}

BOOST_AUTO_TEST_CASE(copyConstructorCreatesDeepCopy)
{
  burukov::AVLTree< int, std::string > t1;
  t1.push(1, "one");
  t1.push(2, "two");
  burukov::AVLTree< int, std::string > t2(t1);
  BOOST_CHECK_EQUAL(t2.size(), 2);
  t2.at(1) = "ONE";
  BOOST_CHECK_EQUAL(t1.at(1), "one");
}

BOOST_AUTO_TEST_CASE(moveConstructorTransfersOwnership)
{
  burukov::AVLTree< int, std::string > t1;
  t1.push(1, "one");
  burukov::AVLTree< int, std::string > t2(std::move(t1));
  BOOST_CHECK_EQUAL(t2.size(), 1);
  BOOST_CHECK(t1.empty());
}

BOOST_AUTO_TEST_CASE(copyAssignmentCreatesDeepCopy)
{
  burukov::AVLTree< int, std::string > t1;
  burukov::AVLTree< int, std::string > t2;
  t1.push(1, "one");
  t2 = t1;
  BOOST_CHECK_EQUAL(t2.size(), 1);
}

BOOST_AUTO_TEST_CASE(moveAssignmentTransfersOwnership)
{
  burukov::AVLTree< int, std::string > t1;
  burukov::AVLTree< int, std::string > t2;
  t1.push(1, "one");
  t2 = std::move(t1);
  BOOST_CHECK_EQUAL(t2.size(), 1);
  BOOST_CHECK(t1.empty());
}

BOOST_AUTO_TEST_CASE(traversalInOrder)
{
  burukov::AVLTree< int, std::string > t;
  t.push(3, "c");
  t.push(1, "a");
  t.push(2, "b");
  t.push(5, "e");
  t.push(4, "d");
  int expected[] = {1, 2, 3, 4, 5};
  int i = 0;
  t.traverseInOrder(
    [&](const int& key, const std::string&)
    {
      BOOST_CHECK_EQUAL(key, expected[i]);
      ++i;
    }
  );
}

BOOST_AUTO_TEST_CASE(clearRemovesAllElements)
{
  burukov::AVLTree< int, std::string > t;
  t.push(1, "one");
  t.push(2, "two");
  t.clear();
  BOOST_CHECK(t.empty());
  BOOST_CHECK_EQUAL(t.size(), 0);
}

BOOST_AUTO_TEST_CASE(stressLargeTree)
{
  const int kStressSize = 500;
  burukov::AVLTree< int, std::string > t;
  for (int i = 0; i < kStressSize; ++i)
  {
    t.push(i, std::to_string(i));
  }
  BOOST_CHECK_EQUAL(t.size(), kStressSize);
  for (int i = 0; i < kStressSize; ++i)
  {
    BOOST_CHECK_EQUAL(t.at(i), std::to_string(i));
  }
  for (int i = 0; i < kStressSize; ++i)
  {
    t.drop(i);
  }
  BOOST_CHECK(t.empty());
}

BOOST_AUTO_TEST_SUITE_END()
