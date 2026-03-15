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
