#include <boost/test/unit_test.hpp>
#include "math_func.hpp"

BOOST_AUTO_TEST_CASE(BitRightShiftBasicTest)
{
  std::string input = "( 15 + 7 >> 2 ) - 3\n42 >> 1 >> 1\n";
  std::istringstream iss(input);
  sedov::Stack< sedov::Queue< std::string > > infix;
  sedov::getInfix(iss, infix);
  sedov::List< std::string > out;

  while (!infix.empty())
  {
    sedov::Queue< std::string > inf = infix.top();
    infix.pop();
    sedov::Queue< std::string > postfix = sedov::convertInfToPost(inf);
    std::string res = sedov::calculate(postfix);
    out.pushBack(res);
  }

  std::string result;
  sedov::LIter< std::string > it = out.begin();
  result += *it;
  ++it;

  for (; it != out.end(); ++it)
  {
    result += ' ';
    result += *it;
  }
  BOOST_CHECK(result == "10 2");
}

BOOST_AUTO_TEST_CASE(BitRightShiftCornerCases)
{
  {
    std::string input = "0 >> 5\n";
    std::istringstream iss(input);
    sedov::Stack< sedov::Queue< std::string > > infix;
    sedov::getInfix(iss, infix);
    sedov::Queue< std::string > postfix = sedov::convertInfToPost(infix.top());
    std::string res = sedov::calculate(postfix);
    BOOST_CHECK(res == "0");
  }

  {
    std::string input = "42 >> 0\n";
    std::istringstream iss(input);
    sedov::Stack< sedov::Queue< std::string > > infix;
    sedov::getInfix(iss, infix);
    sedov::Queue< std::string > postfix = sedov::convertInfToPost(infix.top());
    std::string res = sedov::calculate(postfix);
    BOOST_CHECK(res == "42");
  }

  {
    std::string input = "-8 >> 2\n";
    std::istringstream iss(input);
    sedov::Stack< sedov::Queue< std::string > > infix;
    sedov::getInfix(iss, infix);
    sedov::Queue< std::string > postfix = sedov::convertInfToPost(infix.top());
    std::string res = sedov::calculate(postfix);
    BOOST_CHECK(res == "-2");
  }

  {
    std::string input = "9223372036854775807 >> 1\n";
    std::istringstream iss(input);
    sedov::Stack< sedov::Queue< std::string > > infix;
    sedov::getInfix(iss, infix);
    sedov::Queue< std::string > postfix = sedov::convertInfToPost(infix.top());
    std::string res = sedov::calculate(postfix);
    BOOST_CHECK(res == "4611686018427387903");
  }

  {
    std::string input = "100 >> 2 >> 1\n";
    std::istringstream iss(input);
    sedov::Stack< sedov::Queue< std::string > > infix;
    sedov::getInfix(iss, infix);
    sedov::Queue< std::string > postfix = sedov::convertInfToPost(infix.top());
    std::string res = sedov::calculate(postfix);
    BOOST_CHECK(res == "12");
  }

  {
    std::string input = "( 10 + 6 ) >> 2\n";
    std::istringstream iss(input);
    sedov::Stack< sedov::Queue< std::string > > infix;
    sedov::getInfix(iss, infix);
    sedov::Queue< std::string > postfix = sedov::convertInfToPost(infix.top());
    std::string res = sedov::calculate(postfix);
    BOOST_CHECK(res == "4");
  }
}

BOOST_AUTO_TEST_CASE(BitRightShiftInvalidInput)
{
  {
    std::string input = "42 >> -1\n";
    std::istringstream iss(input);
    sedov::Stack< sedov::Queue< std::string > > infix;
    sedov::getInfix(iss, infix);
    sedov::Queue< std::string > postfix = sedov::convertInfToPost(infix.top());
    BOOST_CHECK_THROW(sedov::calculate(postfix), std::invalid_argument);
  }

  {
    std::string input = "42 >> 64\n";
    std::istringstream iss(input);
    sedov::Stack< sedov::Queue< std::string > > infix;
    sedov::getInfix(iss, infix);
    sedov::Queue< std::string > postfix = sedov::convertInfToPost(infix.top());
    BOOST_CHECK_THROW(sedov::calculate(postfix), std::out_of_range);
  }

  {
    std::string input = "abc >> 2\n";
    std::istringstream iss(input);
    sedov::Stack< sedov::Queue< std::string > > infix;
    sedov::getInfix(iss, infix);
    sedov::Queue< std::string > postfix = sedov::convertInfToPost(infix.top());
    BOOST_CHECK_THROW(sedov::calculate(postfix), std::invalid_argument);
  }
}
