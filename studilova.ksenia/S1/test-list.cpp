#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(ConstructorSuite)

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  studilova::List<int> list;

  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);
}

BOOST_AUTO_TEST_CASE(CopyConstructorEmpty)
{
  studilova::List<int> list;
  studilova::List<int> copy(list);

  BOOST_CHECK(copy.empty());
  BOOST_CHECK_EQUAL(copy.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()
