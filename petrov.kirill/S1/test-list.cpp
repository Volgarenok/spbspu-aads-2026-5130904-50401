#define BOOST_TEST_MODULE ListTest
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(ConstructionSuite)

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  petrov::List< int > list;
  BOOST_CHECK(list.isEmpty());
  BOOST_CHECK_EQUAL(list.getSize(), 0);
}

BOOST_AUTO_TEST_CASE(TestEmptyList)
{
  petrov::List< int > list;
  BOOST_CHECK(list.isEmpty());
  BOOST_CHECK_EQUAL(list.getSize(), 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(TestPushBack)
{
  petrov::List< int > list;
  list.pushBack(-1);
  list.pushBack(-33);
  list.pushBack(27);
  list.pushBack(77);
  list.pushBack(9);

  BOOST_CHECK(!list.isEmpty());
  BOOST_CHECK_EQUAL(list.getSize(), 5);
  BOOST_CHECK_EQUAL(list.getBack(), 9);
}

BOOST_AUTO_TEST_CASE(TestClear)
{
  petrov::List< int > list;
  list.pushBack(0);
  list.pushBack(1);
  list.pushBack(-1);
  list.clear();

  BOOST_CHECK(list.isEmpty());
  BOOST_CHECK_EQUAL(list.getSize(), 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(ListIteratorTests)

BOOST_AUTO_TEST_CASE(TestIncrement)
{
  petrov::List< int > list;
  list.pushBack(1);
  list.pushBack(2);

  petrov::List< int >::Iterator i = list.begin();

  BOOST_REQUIRE(i != list.end());
  BOOST_CHECK_EQUAL(*i, 1);

  ++i;
  BOOST_REQUIRE(i != list.end());
  BOOST_CHECK_EQUAL(*i, 2);

  ++i;
  BOOST_CHECK(i == list.end());
}

BOOST_AUTO_TEST_CASE(TestConstIterator)
{
  petrov::List< int > list;
  list.pushBack(1);

  const petrov::List< int > & const_list = list;

  petrov::List< int >::ConstIterator i = const_list.begin();
  
  BOOST_CHECK_EQUAL(*i, 1);
}

BOOST_AUTO_TEST_SUITE_END()
