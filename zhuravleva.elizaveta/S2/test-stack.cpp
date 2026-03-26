#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include "stack.hpp"

BOOST_AUTO_TEST_CASE(stack_push_drop)
{
  zhuravleva::Stack< int > s;
  s.push(1);
  s.push(2);
  s.push(3);
  BOOST_CHECK_EQUAL(s.top(), 3);
  s.pop();
  BOOST_CHECK_EQUAL(s.top(), 2);
  s.pop();
  BOOST_CHECK_EQUAL(s.top(), 1);
  s.pop();
}

BOOST_AUTO_TEST_CASE(stack_empty)
{
  zhuravleva::Stack< int > s;
  BOOST_CHECK(s.empty());
  s.push(10);
  BOOST_CHECK(!s.empty());
  s.pop();
  BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_CASE(stack_clear)
{
  zhuravleva::Stack< int > s;
  s.push(1);
  s.push(2);
  s.push(3);
  s.clear();
  BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_CASE(stack_exception_pop)
{
  zhuravleva::Stack< int > s;
  BOOST_CHECK_THROW(s.pop(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(stack_exception_top)
{
  zhuravleva::Stack< int > s;
  BOOST_CHECK_THROW(s.top(), std::runtime_error);
}


BOOST_AUTO_TEST_CASE(stack_check)
{
  zhuravleva::Stack< int > s;
  s.push(5);
  s.push(10);
  BOOST_CHECK_EQUAL(s.top(), 10);
  s.pop();
  BOOST_CHECK_EQUAL(s.top(), 5);
}
