#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(ConstructorSuite)

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  studilova::List< int > list;

  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);
}

BOOST_AUTO_TEST_CASE(CopyConstructorEmpty)
{
  studilova::List< int > list;
  studilova::List< int > copy(list);

  BOOST_CHECK(copy.empty());
  BOOST_CHECK_EQUAL(copy.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(InsertSuite)

BOOST_AUTO_TEST_CASE(InsertSingle)
{
  studilova::List< int > list;

  list.insert(list.end(), 10);

  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 10);
  BOOST_CHECK_EQUAL(list.back(), 10);
}

BOOST_AUTO_TEST_CASE(InsertMultiple)
{
  studilova::List< int > list;

  list.insert(list.end(), 1);
  list.insert(list.end(), 2);
  list.insert(list.end(), 3);

  BOOST_CHECK_EQUAL(list.size(), 3);
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 3);
}

BOOST_AUTO_TEST_CASE(InsertAtBegin)
{
  studilova::List< int > list;

  list.insert(list.end(), 2);
  list.insert(list.begin(), 1);

  BOOST_CHECK_EQUAL(list.front(), 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(EraseSuite)

BOOST_AUTO_TEST_CASE(PopFront)
{
  studilova::List< int > list;

  list.insert(list.end(), 1);
  list.insert(list.end(), 2);

  list.popFront();

  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 2);
}

BOOST_AUTO_TEST_CASE(PopBack)
{
  studilova::List< int > list;

  list.insert(list.end(), 1);
  list.insert(list.end(), 2);

  list.popBack();

  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.back(), 1);
}

BOOST_AUTO_TEST_CASE(Clear)
{
  studilova::List< int > list;

  list.insert(list.end(), 1);
  list.insert(list.end(), 2);

  list.clear();

  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(ExceptionSuite)

BOOST_AUTO_TEST_CASE(FrontEmpty)
{
  studilova::List< int > list;

  BOOST_CHECK_THROW(list.front(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(BackEmpty)
{
  studilova::List< int > list;

  BOOST_CHECK_THROW(list.back(), std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(AdditionalTaskSuite)

BOOST_AUTO_TEST_CASE(SpliceAll)
{
  studilova::List< int > first;
  first.pushBack(1);
  first.pushBack(2);

  studilova::List< int > second;
  second.pushBack(10);
  second.pushBack(20);

  first.splice(first.begin(), second);

  BOOST_CHECK_EQUAL(first.size(), 4);
  BOOST_CHECK(second.empty());

  auto it = first.begin();
  BOOST_CHECK_EQUAL(*it, 10);
  ++it;
  BOOST_CHECK_EQUAL(*it, 20);
  ++it;
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
}

BOOST_AUTO_TEST_CASE(SpliceOne)
{
  studilova::List< int > first;
  first.pushBack(1);
  first.pushBack(3);

  studilova::List< int > second;
  second.pushBack(10);
  second.pushBack(20);

  auto it = second.begin();
  ++it;

  first.splice(first.begin(), second, it);

  BOOST_CHECK_EQUAL(first.size(), 3);
  BOOST_CHECK_EQUAL(second.size(), 1);

  BOOST_CHECK_EQUAL(first.front(), 20);
  BOOST_CHECK_EQUAL(second.front(), 10);
}

BOOST_AUTO_TEST_CASE(SortDefault)
{
  studilova::List< int > list;
  list.pushBack(5);
  list.pushBack(1);
  list.pushBack(4);
  list.pushBack(2);
  list.pushBack(3);

  list.sort();

  auto it = list.begin();
  for (int expected = 1; expected <= 5; ++expected, ++it)
  {
    BOOST_CHECK_EQUAL(*it, expected);
  }
}

BOOST_AUTO_TEST_CASE(SortWithComparator)
{
  studilova::List< int > list;
  list.pushBack(1);
  list.pushBack(5);
  list.pushBack(3);

  list.sort([](int lhs, int rhs)
  {
    return lhs > rhs;
  });

  auto it = list.begin();

  BOOST_CHECK_EQUAL(*it, 5);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  ++it;
  BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(MergeDefault)
{
  studilova::List< int > first;
  first.pushBack(1);
  first.pushBack(3);
  first.pushBack(5);

  studilova::List< int > second;
  second.pushBack(2);
  second.pushBack(4);
  second.pushBack(6);

  first.merge(second);

  BOOST_CHECK(second.empty());

  auto it = first.begin();
  for (int expected = 1; expected <= 6; ++expected, ++it)
  {
    BOOST_CHECK_EQUAL(*it, expected);
  }
}

BOOST_AUTO_TEST_CASE(MergeWithComparator)
{
  studilova::List< int > first;
  first.pushBack(6);
  first.pushBack(4);
  first.pushBack(2);

  studilova::List< int > second;
  second.pushBack(5);
  second.pushBack(3);
  second.pushBack(1);

  first.merge(second, [](int lhs, int rhs)
  {
    return lhs > rhs;
  });

  int expected[] = {6, 5, 4, 3, 2, 1};

  auto it = first.begin();
  for (size_t i = 0; i < first.size(); ++i, ++it)
  {
    BOOST_CHECK_EQUAL(*it, expected[i]);
  }

  BOOST_CHECK(second.empty());
}

BOOST_AUTO_TEST_CASE(Partition)
{
  studilova::List< int > list;
  list.pushBack(3);
  list.pushBack(1);
  list.pushBack(4);
  list.pushBack(1);
  list.pushBack(5);
  list.pushBack(9);
  list.pushBack(2);
  list.pushBack(6);

  auto splitPoint = list.partition([](int value)
  {
    return value % 2 == 0;
  });

  int expected[] = {4, 2, 6, 3, 1, 1, 5, 9};

  auto it = list.begin();
  for (size_t i = 0; i < list.size(); ++i, ++it)
  {
    BOOST_CHECK_EQUAL(*it, expected[i]);
  }

  BOOST_CHECK(splitPoint != list.end());
  BOOST_CHECK_EQUAL(*splitPoint, 3);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(IteratorSuite)

BOOST_AUTO_TEST_CASE(IteratorIncrement)
{
  studilova::List< int > list;

  list.insert(list.end(), 1);
  list.insert(list.end(), 2);
  list.insert(list.end(), 3);

  auto it = list.begin();

  BOOST_CHECK_EQUAL(*it, 1);

  ++it;
  BOOST_CHECK_EQUAL(*it, 2);

  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
}

BOOST_AUTO_TEST_CASE(IteratorDecrement)
{
  studilova::List< int > list;

  list.insert(list.end(), 1);
  list.insert(list.end(), 2);
  list.insert(list.end(), 3);

  auto it = list.begin();

  --it;
  BOOST_CHECK_EQUAL(*it, 3);
}

BOOST_AUTO_TEST_CASE(IteratorComparison)
{
  studilova::List< int > list;

  list.insert(list.end(), 1);
  list.insert(list.end(), 2);

  auto it1 = list.begin();
  auto it2 = list.begin();

  BOOST_CHECK(it1 == it2);

  ++it2;

  BOOST_CHECK(it1 != it2);
}

BOOST_AUTO_TEST_SUITE_END()
