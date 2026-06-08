#include <iostream>
#include <boost/test/unit_test.hpp>
#include <queue.hpp>
#include <stack.hpp>

#include "math_funcs.hpp"

BOOST_AUTO_TEST_CASE (BitShiftToRightTest)
{
  namespace kuz = kuznetsov;
  std::string inp = "( 125 >> 2 ) + 9 * 3 - ( 100 >> 3 ) / 4\n64 >> 2\n( 15 + 5 ) / ( 28 / 7 )\n";
  std::istringstream input(inp);

  kuz::Stack< kuz::InfixExpression > mathExpressions = kuz::getExpressions(input);
  kuz::Queue< kuz::lli_t > res = kuz::calculateStackOfInfix(mathExpressions);
  std::string result = "";
  result += std::to_string(res.front());
  res.pop();
  result += ' ';
  result += std::to_string(res.front());
  res.pop();
  result += ' ';
  result += std::to_string(res.front());
  res.pop();
  BOOST_CHECK(result == "5 16 55");
}

BOOST_AUTO_TEST_CASE (BitShiftToRightZeroOperandTest)
{
  namespace kuz = kuznetsov;
  std::string inp = "0 >> 5\n0 >> 1\n( 0 >> 3 ) + ( 0 >> 7 )\n";
  std::istringstream input(inp);
  kuz::Stack< kuz::InfixExpression > mathExpressions = kuz::getExpressions(input);
  kuz::Queue< kuz::lli_t > res = kuz::calculateStackOfInfix(mathExpressions);
  BOOST_CHECK_EQUAL(res.front(), 0);
  res.pop();
  BOOST_CHECK_EQUAL(res.front(), 0);
  res.pop();
  BOOST_CHECK_EQUAL(res.front(), 0);
  res.pop();
}

BOOST_AUTO_TEST_CASE (BitShiftToRightLargeShiftTest)
{
  namespace kuz = kuznetsov;
  std::string inp = "5 >> 100\n1 >> 200\n";
  std::istringstream input(inp);
  kuz::Stack< kuz::InfixExpression > mathExpressions = kuz::getExpressions(input);
  kuz::Queue< kuz::lli_t > res = kuz::calculateStackOfInfix(mathExpressions);
  BOOST_CHECK_EQUAL(res.front(), 0);
  res.pop();
  BOOST_CHECK_EQUAL(res.front(), 0);
  res.pop();
}

BOOST_AUTO_TEST_CASE (BitShiftBoundaryTest)
{
  namespace kuz = kuznetsov;
  std::string inp = "42 >> 0\n5 >> 63\n5 >> 64\n5 >> 1000\n";
  std::istringstream input(inp);

  kuz::Stack< kuz::InfixExpression > mathExpressions = kuz::getExpressions(input);
  kuz::Queue< kuz::lli_t > res = kuz::calculateStackOfInfix(mathExpressions);

  BOOST_CHECK_EQUAL(res.front(), 0);
  res.pop();
  BOOST_CHECK_EQUAL(res.front(), 0);
  res.pop();
  BOOST_CHECK_EQUAL(res.front(), 0);
  res.pop();
  BOOST_CHECK_EQUAL(res.front(), 42);
  res.pop();
}

BOOST_AUTO_TEST_CASE (BitShiftNegativeThrowsTest)
{
  namespace kuz = kuznetsov;
  std::string inp = "100 >> ( 0 - 2 )\n";
  std::istringstream input(inp);

  kuz::Stack< kuz::InfixExpression > mathExpressions = kuz::getExpressions(input);

  BOOST_CHECK_THROW(kuz::calculateStackOfInfix(mathExpressions), std::logic_error);
}
