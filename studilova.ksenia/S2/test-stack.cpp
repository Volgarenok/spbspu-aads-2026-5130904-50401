#include <string>

#include <boost/test/unit_test.hpp>

#include "stack.hpp"
#include "math-expression.hpp"

BOOST_AUTO_TEST_SUITE(stack_tests)

BOOST_AUTO_TEST_CASE(push_and_pop)
{
  studilova::Stack< int > s;

  s.push(1);
  s.push(2);
  s.push(3);

  BOOST_CHECK_EQUAL(s.top(), 3);
  s.pop();

  BOOST_CHECK_EQUAL(s.top(), 2);
  s.pop();

  BOOST_CHECK_EQUAL(s.top(), 1);
  s.pop();

  BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_CASE(top_does_not_remove)
{
  studilova::Stack< int > s;

  s.push(10);
  s.push(20);

  BOOST_CHECK_EQUAL(s.top(), 20);
  BOOST_CHECK_EQUAL(s.top(), 20);
  BOOST_CHECK_EQUAL(s.size(), 2);
}

BOOST_AUTO_TEST_CASE(empty_and_size)
{
  studilova::Stack< int > s;

  BOOST_CHECK(s.empty());
  BOOST_CHECK_EQUAL(s.size(), 0);

  s.push(1);
  BOOST_CHECK(!s.empty());
  BOOST_CHECK_EQUAL(s.size(), 1);
}

BOOST_AUTO_TEST_CASE(pop_empty_throws)
{
  studilova::Stack< int > s;

  BOOST_CHECK_THROW(s.pop(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(top_empty_throws)
{
  studilova::Stack< int > s;

  BOOST_CHECK_THROW(s.top(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(string_stack)
{
  studilova::Stack< std::string > s;

  s.push("hello");
  s.push("world");

  BOOST_CHECK_EQUAL(s.top(), "world");
  s.pop();

  BOOST_CHECK_EQUAL(s.top(), "hello");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(math_expression_tests)

BOOST_AUTO_TEST_CASE(power_simple)
{
  BOOST_CHECK_EQUAL(studilova::evaluateExpression("2 ** 3"), 8);
}

BOOST_AUTO_TEST_CASE(power_priority)
{
  BOOST_CHECK_EQUAL(studilova::evaluateExpression("2 + 3 ** 2"), 11);
}

BOOST_AUTO_TEST_CASE(power_right_associative)
{
  BOOST_CHECK_EQUAL(studilova::evaluateExpression("2 ** 3 ** 2"), 512);
}

BOOST_AUTO_TEST_CASE(power_with_parentheses)
{
  BOOST_CHECK_EQUAL(studilova::evaluateExpression("( 2 ** 3 ) ** 2"), 64);
}

BOOST_AUTO_TEST_CASE(power_zero_exponent)
{
  BOOST_CHECK_EQUAL(studilova::evaluateExpression("5 ** 0"), 1);
}

BOOST_AUTO_TEST_SUITE_END()
