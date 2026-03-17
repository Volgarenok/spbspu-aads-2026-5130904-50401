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
