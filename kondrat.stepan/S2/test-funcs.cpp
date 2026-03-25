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

BOOST_AUTO_TEST_CASE(GetOperatorType)
{
  BOOST_CHECK(getOperatorType("1") == 0);
  BOOST_CHECK(getOperatorType("+") == 1);
  BOOST_CHECK(getOperatorType("-") == 1);
  BOOST_CHECK(getOperatorType("*") == 1);
  BOOST_CHECK(getOperatorType("/") == 1);
  BOOST_CHECK(getOperatorType("%") == 1);
  BOOST_CHECK(getOperatorType("#") == 2);
}

BOOST_AUTO_TEST_CASE(IsOperator)
{
  BOOST_CHECK(isOperator("1") == false);
  BOOST_CHECK(isOperator("+") == true);
  BOOST_CHECK(isOperator("-") == true);
  BOOST_CHECK(isOperator("*") == true);
  BOOST_CHECK(isOperator("/") == true);
  BOOST_CHECK(isOperator("%") == true);
  BOOST_CHECK(isOperator("#") == true);
}

BOOST_AUTO_TEST_CASE(GetPriority)
{
  BOOST_CHECK(getPriority("#") == 3);
  BOOST_CHECK(getPriority("*") == 2);
  BOOST_CHECK(getPriority("/") == 2);
  BOOST_CHECK(getPriority("%") == 2);
  BOOST_CHECK(getPriority("+") == 1);
  BOOST_CHECK(getPriority("-") == 1);
  BOOST_CHECK(getPriority("1") == 0);
}

BOOST_AUTO_TEST_CASE(Tokenize)
{
  Queue< std::string > res = tokenize("2 + 2 * 33 + # 12");

  BOOST_CHECK(res.size() == 8);
  BOOST_CHECK(res.first() == "2");
  BOOST_CHECK(res.last() == "12");
}

BOOST_AUTO_TEST_CASE(Postfix)
{
  Queue< std::string > res = tokenize("2 + 2 * 33 + # 12");

  res = infixToPostfix(res);

  BOOST_CHECK(res.size() == 8);
  BOOST_CHECK(res.first() == "2");
  BOOST_CHECK(res.last() == "+");

  res.drop();
  res.drop();

  BOOST_CHECK(res.drop() == "33");
  BOOST_CHECK(res.drop() == "*");
  BOOST_CHECK(res.drop() == "+");
}

BOOST_AUTO_TEST_CASE(ReadEvaluatePostfixPrint)
{
  Stack< ll > res;

  std::stringstream in1("2 + 2 * 2 * # 10 * ( 5 + 1 )");
  std::stringstream in2("2 + 2 * 2");
  readExpressions(in1, res);

  BOOST_CHECK(res.size() == 1);
  BOOST_CHECK(res.first() == 26);

  readExpressions(in2, res);

  std::ostringstream out;
  printResults(res, out);

  std::string done = out.str();
  BOOST_CHECK(done == "6 26\n");
}

BOOST_AUTO_TEST_SUITE_END()
