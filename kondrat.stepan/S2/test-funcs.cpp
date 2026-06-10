#include <boost/test/unit_test.hpp>
#include "math-funcs.hpp"

using namespace kondrat;

BOOST_AUTO_TEST_SUITE(FuncsTestSuite)

BOOST_AUTO_TEST_CASE(ReverseNumber)
{
  ll num1 = 123;
  ll num2 = -123;
  ll num3 = 0;

  BOOST_CHECK(reverseNumber(num1) == 321);
  BOOST_CHECK(reverseNumber(num2) == -321);
  BOOST_CHECK(reverseNumber(num3) == 0);
}

BOOST_AUTO_TEST_CASE(BinaryOperations)
{
  ll num1 = 123;
  ll num2 = 10;

  BOOST_CHECK(applyBinaryOperation(num1, num2, "+") == 133);
  BOOST_CHECK(applyBinaryOperation(num1, num2, "-") == 113);
  BOOST_CHECK(applyBinaryOperation(num1, num2, "*") == 1230);
  BOOST_CHECK(applyBinaryOperation(num1, num2, "/") == 12);
  BOOST_CHECK(applyBinaryOperation(num1, num2, "%") == 3);
  BOOST_CHECK(applyUnaryOperation(num2, "#") == 1);
}

BOOST_AUTO_TEST_CASE(CalculateExpression)
{
  BOOST_CHECK(calculateExpression("2 + 2 * 2") == 6);
  BOOST_CHECK(calculateExpression("2 + 2 * 2 * # 10 * ( 5 + 1 )") == 26);
  BOOST_CHECK(calculateExpression("10 % 4") == 2);
}

BOOST_AUTO_TEST_SUITE_END()
