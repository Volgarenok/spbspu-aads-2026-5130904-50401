#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_SUITE(stack_suite)

BOOST_AUTO_TEST_CASE(test_create_stack)
{
  hvostov::Stack< int > stack;
  BOOST_CHECK(stack.empty());
}

BOOST_AUTO_TEST_CASE(test_push_element)
{
  hvostov::Stack< int > stack;
  stack.push(6);
  BOOST_CHECK(!stack.empty());
  BOOST_CHECK(stack.top() == 6);
  stack.push(7);
  BOOST_CHECK(stack.top() == 7);
  stack.push(8);
  BOOST_CHECK(stack.top() == 8);
}

BOOST_AUTO_TEST_CASE(test_drop_element)
{
  hvostov::Stack< int > stack;
  stack.push(6);
  stack.push(7);
  stack.push(8);
  BOOST_CHECK(stack.top() == 8);
  stack.drop();
  BOOST_CHECK(stack.top() == 7);
  stack.drop();
  BOOST_CHECK(stack.top() == 6);
  stack.drop();
  BOOST_CHECK(stack.empty());
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  hvostov::Stack< int > stack;
  stack.push(6);
  stack.push(7);
  stack.push(8);
  BOOST_CHECK(!stack.empty());
  stack.clear();
  BOOST_CHECK(stack.empty());
  stack.push(6);
  stack.push(7);
  BOOST_CHECK(!stack.empty());
  stack.clear();
  BOOST_CHECK(stack.empty());
}

BOOST_AUTO_TEST_SUITE_END()
