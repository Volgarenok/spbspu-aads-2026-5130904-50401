#include <boost/test/unit_test.hpp>
#include <sstream>
#include "mathFunctions.hpp"

BOOST_AUTO_TEST_SUITE(BitwiseAndTest)

BOOST_AUTO_TEST_CASE(BasicValues)
{
  BOOST_CHECK(burukov::bitwiseAnd(12, 10) == 8);
  BOOST_CHECK(burukov::bitwiseAnd(7, 7) == 7);
  BOOST_CHECK(burukov::bitwiseAnd(8, 6) == 0);
  BOOST_CHECK(burukov::bitwiseAnd(0, 255) == 0);
  BOOST_CHECK(burukov::bitwiseAnd(15, 9) == 9);
  BOOST_CHECK(burukov::bitwiseAnd(-1, 7) == 7);
}

BOOST_AUTO_TEST_CASE(ExpressionEvaluation)
{
  std::string input = "( 3 + 5 ) & 6\n7 & 5\n";
  std::istringstream iss(input);
  burukov::Stack< burukov::Queue< std::string > > infix;
  burukov::getInfix(iss, infix);

  burukov::Queue< std::string > results;
  while (!infix.empty())
  {
    const burukov::Queue< std::string > inf = infix.top();
    infix.pop();
    burukov::Queue< std::string > postfix;
    burukov::convertToPostfix(inf, postfix);
    const std::string res = burukov::calculate(postfix);
    results.push(res);
  }

  std::string output;
  output += results.front();
  results.pop();
  while (!results.empty())
  {
    output += ' ';
    output += results.front();
    results.pop();
  }
  BOOST_CHECK(output == "5 0");
}

BOOST_AUTO_TEST_CASE(PriorityCheck)
{
  std::string input = "2 + 3 & 7\n";
  std::istringstream iss(input);
  burukov::Stack< burukov::Queue< std::string > > infix;
  burukov::getInfix(iss, infix);

  const burukov::Queue< std::string > inf = infix.top();
  burukov::Queue< std::string > postfix;
  burukov::convertToPostfix(inf, postfix);
  const std::string res = burukov::calculate(postfix);
  BOOST_CHECK(res == "5");
}

BOOST_AUTO_TEST_CASE(AndWithParentheses)
{
  std::string input = "15 & ( 3 + 4 )\n";
  std::istringstream iss(input);
  burukov::Stack< burukov::Queue< std::string > > infix;
  burukov::getInfix(iss, infix);

  const burukov::Queue< std::string > inf = infix.top();
  burukov::Queue< std::string > postfix;
  burukov::convertToPostfix(inf, postfix);
  const std::string res = burukov::calculate(postfix);
  BOOST_CHECK(res == "7");
}

BOOST_AUTO_TEST_SUITE_END()
