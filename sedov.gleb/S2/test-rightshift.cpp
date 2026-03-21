#include <boost/test/unit_test.hpp>
#include "math_func.hpp"

BOOST_AUtO_TEST_CASE(BitRightShiftTest)
{
  std::string input = "( 15 + 7  >> 2 ) - 3\n42 >> 1 >> 1\n";
  std::istringstream iss(input);
  sedov::Stack< sedov::Queue< std::string > > infix;
  getInfix(iss, infix);
  sedov::List< std::string > out;

  while (!infix.empty())
  {
    sedov::Queue< std::string > inf = infix.top();
    infix.pop();
    sedov::Queue< std::string > postfix;
    convertInfToPost(inf, postfix);
    std::string res;
    res = sedov::calculate(postfix);
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
