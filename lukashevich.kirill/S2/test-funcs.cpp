#include <boost/test/unit_test.hpp>
#include "math_funcs.hpp"

using namespace lukashevich;

BOOST_AUTO_TEST_SUITE(FuncsTestSuite)

BOOST_AUTO_TEST_CASE(HelperFunctions)
{
  BOOST_CHECK(detail::getOperatorType("5") == 0);
  BOOST_CHECK(detail::getOperatorType("+") == 1);
  BOOST_CHECK(detail::getOperatorType("!") == 2);
  BOOST_CHECK(detail::isOperator("*") == true);
  BOOST_CHECK(detail::isOperator("10") == false);
  BOOST_CHECK(detail::getPriority("!") == 3);
  BOOST_CHECK(detail::getPriority("/") == 2);
  BOOST_CHECK(detail::getPriority("-") == 1);
  BOOST_CHECK(detail::getPriority("abc") == 0);
}

BOOST_AUTO_TEST_CASE(TokenizeTest)
{
  List< std::string > res = tokenize("2 + 2 * 33 + ! 12");

  BOOST_CHECK(res.size() == 8);
  BOOST_CHECK(res.front() == "2");
  BOOST_CHECK(res.back() == "12");
}

BOOST_AUTO_TEST_CASE(InfixToPostfixTest)
{
  List< std::string > tokens = tokenize("2 + 2 * 33");
  Queue< std::string > res = infixToPostfix(tokens);

  BOOST_CHECK(res.size() == 5);

  BOOST_CHECK(res.front() == "2");
  res.pop();

  BOOST_CHECK(res.front()== "2");
  res.pop();

  BOOST_CHECK(res.front() == "33");
  res.pop();

  BOOST_CHECK(res.front()== "*");
  res.pop();

  BOOST_CHECK(res.front() == "+");
}

BOOST_AUTO_TEST_CASE(ArithmeticOperations)
{
  BOOST_CHECK(add(10, 20) == 30);
  BOOST_CHECK(sub(30, 10) == 20);
  BOOST_CHECK(mul(5, 6) == 30);
  BOOST_CHECK(divide(100, 10) == 10);
  BOOST_CHECK(mod(10, 3) == 1);
  BOOST_CHECK(bitwise_not(5) == -6);
}

BOOST_AUTO_TEST_CASE(OverflowDetection)
{
  BOOST_CHECK_THROW(add(std::numeric_limits<ll>::max(), 1), std::overflow_error);
  BOOST_CHECK_THROW(sub(std::numeric_limits<ll>::min(), 1), std::overflow_error);
  BOOST_CHECK_THROW(mul(std::numeric_limits<ll>::max(), 2), std::overflow_error);
  BOOST_CHECK_THROW(divide(std::numeric_limits<ll>::min(), -1), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(EvaluateExpressionIntegration)
{
  BOOST_CHECK(evaluate_expression("1 + 2") == 3);
  BOOST_CHECK(evaluate_expression("4 * 7 - 3") == 25);
  BOOST_CHECK(evaluate_expression("( 10 / ( 2 + 3 ) ) % 4") == 2);
  BOOST_CHECK(evaluate_expression("! 5") == -6);
  BOOST_CHECK(evaluate_expression("! 0") == -1);
  BOOST_CHECK_THROW(evaluate_expression("1 / 0"), std::runtime_error);
  BOOST_CHECK_THROW(evaluate_expression("5 +"), std::runtime_error);
  BOOST_CHECK_THROW(evaluate_expression("9223372036854775807 + 1"), std::overflow_error);
}

BOOST_AUTO_TEST_SUITE_END()
