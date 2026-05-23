#include <boost/test/unit_test.hpp>
#include <string>
#include "stack.hpp"

BOOST_AUTO_TEST_SUITE(StackTests)

BOOST_AUTO_TEST_CASE(stack_push_and_drop)
{
  vasyakin::Stack< int > stack;

  stack.push(1);
  stack.push(2);
  stack.push(3);

  BOOST_CHECK_EQUAL(stack.top(), 3);
  stack.pop();

  BOOST_CHECK_EQUAL(stack.top(), 2);
  stack.pop();

  BOOST_CHECK_EQUAL(stack.top(), 1);
  stack.pop();
}

BOOST_AUTO_TEST_CASE(stack_top)
{
  vasyakin::Stack< int > stack;

  stack.push(10);
  stack.push(20);

  BOOST_CHECK_EQUAL(stack.top(), 20);
  BOOST_CHECK_EQUAL(stack.size(), 2);
}

BOOST_AUTO_TEST_CASE(stack_empty_and_size)
{
  vasyakin::Stack< int > stack;

  BOOST_CHECK(stack.empty());
  BOOST_CHECK_EQUAL(stack.size(), 0);

  stack.push(1);
  BOOST_CHECK(!stack.empty());
  BOOST_CHECK_EQUAL(stack.size(), 1);
}

BOOST_AUTO_TEST_CASE(stack_with_strings)
{
  vasyakin::Stack< std::string > stack;

  stack.push("hello");
  stack.push("world");

  BOOST_CHECK_EQUAL(stack.top(), "world");
  stack.pop();

  BOOST_CHECK_EQUAL(stack.top(), "hello");
  stack.pop();
}

BOOST_AUTO_TEST_SUITE_END()
