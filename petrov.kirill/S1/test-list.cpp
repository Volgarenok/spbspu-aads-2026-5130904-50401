#define BOOST_TEST_MODULE ListDeepTest
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(ListTestForLab)

BOOST_AUTO_TEST_CASE(test_iter)
{
  petrov::List<int> l;
  l.push_back(1);

  petrov::LIter<int> i = l.begin();
  BOOST_CHECK_EQUAL(*i, 1);
}

BOOST_AUTO_TEST_CASE(test_isempty)
{
  petrov::List<int> c;

  BOOST_CHECK_EQUAL(1, c.IsEmpty());
}

BOOST_AUTO_TEST_SUITE_END()
