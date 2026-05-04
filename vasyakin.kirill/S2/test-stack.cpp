#include <boost/test/unit_test.hpp>
#include "stack.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(StackTests)

BOOST_AUTO_TEST_CASE(stack_push_and_drop)
{
  vasyakin::Stack< int > stack;

  stack.push(1);
  stack.push(2);
  stack.push(3);

  BOOST_CHECK_EQUAL(stack.drop(), 3);
  BOOST_CHECK_EQUAL(stack.drop(), 2);
  BOOST_CHECK_EQUAL(stack.drop(), 1);
}

BOOST_AUTO_TEST_CASE(stack_peek)
{
  vasyakin::Stack< int > stack;

  stack.push(10);
  stack.push(20);

  BOOST_CHECK_EQUAL(stack.peek(), 20);
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
  vasyakin::Stack<std::string> stack;

  stack.push("hello");
  stack.push("world");

  BOOST_CHECK_EQUAL(stack.drop(), "world");
  BOOST_CHECK_EQUAL(stack.drop(), "hello");
}

BOOST_AUTO_TEST_SUITE_END()
