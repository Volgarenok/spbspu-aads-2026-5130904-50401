#include "stack.hpp"
#include <boost/test/unit_test.hpp>
#include <stdexcept>

using namespace levkin;
BOOST_AUTO_TEST_CASE(stack_lifo_order)
{
  Stack< int > s;
  s.push(1);
  s.push(2);
  s.push(3);

  BOOST_CHECK_EQUAL(s.top(), 3);
  s.pop();
  BOOST_CHECK_EQUAL(s.top(), 2);
  BOOST_CHECK_EQUAL(s.drop(), 2);
  BOOST_CHECK_EQUAL(s.top(), 1);
}

BOOST_AUTO_TEST_CASE(stack_empty_behavior)
{
  Stack< double > s;
  BOOST_CHECK(s.empty());

  BOOST_CHECK_THROW(s.top(), std::out_of_range);
  BOOST_CHECK_THROW(s.drop(), std::out_of_range);

  s.push(1.1);
  s.pop();
  BOOST_CHECK(s.empty());
  BOOST_CHECK_THROW(s.top(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(stack_large_volume)
{
  Stack< int > s;
  for (int i = 0; i < 1000; ++i) {
    s.push(i);
  }
  BOOST_CHECK_EQUAL(s.size(), 1000);
  BOOST_CHECK_EQUAL(s.top(), 999);

  for (int i = 0; i < 1000; ++i) {
    s.pop();
  }
  BOOST_CHECK(s.empty());
}
