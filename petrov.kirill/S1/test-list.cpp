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

BOOST_AUTO_TEST_CASE(test_empty_plus_elems)
{
  petrov::List<int> c;
  c.push_back(5);

  BOOST_CHECK(!c.IsEmpty());
}

BOOST_AUTO_TEST_CASE(test_push_front)
{
  petrov::List<int> c;
  c.push_front(73);
  c.push_front(37);

  petrov::LIter<int> i = c.begin();
  BOOST_CHECK_EQUAL(*i, 37);
  BOOST_CHECK_EQUAL(*(++i), 73);
}

BOOST_AUTO_TEST_CASE(test_iter_equal)
{
  petrov::List<int> c;
  c.push_back(11);

  petrov::LIter<int> i = c.begin();
  petrov::LIter<int> i2 = c.begin();
  BOOST_CHECK(i == i2);
}

BOOST_AUTO_TEST_CASE(test_iter_not_eq)
{
  petrov::List<int> c;
  c.push_back(11);
  c.push_back(73);

  petrov::LIter<int> i1 = c.begin();
  petrov::LIter<int> i2 = c.begin();
  ++i2;
  BOOST_CHECK(i1 != i2);
}

BOOST_AUTO_TEST_CASE(test_minus_plus)
{
  petrov::List<int> c;
  c.push_back(1);
  c.push_back(2);

  petrov::LIter<int> i = c.begin();
  petrov::LIter<int> i2 = c.begin();
  ++i2;
  BOOST_CHECK(i == --i2);
}

BOOST_AUTO_TEST_CASE(test_nullptr)
{
  petrov::List<int> c;
  c.push_back(1);

  BOOST_CHECK(c.end() == nullptr);
}

BOOST_AUTO_TEST_SUITE_END()
