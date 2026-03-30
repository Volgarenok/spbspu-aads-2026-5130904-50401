#include "stack.hpp"
#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>

using namespace levkin;
BOOST_AUTO_TEST_CASE(stack_drop_push_test)
{
  levkin::Stack< int > a{};
  a.push(1);
  a.push(2);
  a.push(3);
  BOOST_CHECK_EQUAL(a.drop(), 3);
  BOOST_CHECK_EQUAL(a.drop(), 2);
  BOOST_CHECK_EQUAL(a.drop(), 1);
}

BOOST_AUTO_TEST_CASE(stack_pop_front_test)
{
  levkin::Stack< int > a{};
  a.push(1);
  a.push(2);
  a.push(3);
  BOOST_CHECK_EQUAL(a.front(), 3);
  BOOST_CHECK_EQUAL(a.front(), 3);
  a.pop();
  a.pop();
  BOOST_CHECK_EQUAL(a.drop(), 1);
}
