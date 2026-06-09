#include <boost/test/unit_test.hpp>

#include "math_expression.hpp"

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
