#include <boost/test/unit_test.hpp>

#include <sstream>
#include <string>

#include "../common/list.hpp"
#include "math_func.hpp"

BOOST_AUTO_TEST_CASE(bitwiseOrCalculation)
{
  const std::string input = "( 8 + 2 | 5 ) - 3\n42 | 1 | 4\n";
  std::istringstream stream(input);
  yalovsky::Stack< yalovsky::Queue< std::string > > infix;

  yalovsky::getInfix(stream, infix);

  yalovsky::List< std::string > output;
  while (!infix.empty())
  {
    yalovsky::Queue< std::string > postfix;
    yalovsky::convertInfToPost(infix.drop(), postfix);
    output.pushBack(yalovsky::calculate(postfix));
  }

  std::string result;
  yalovsky::List< std::string >::const_iterator it = output.cbegin();
  result += *it;
  ++it;
  for (; it != output.cend(); ++it)
  {
    result += ' ';
    result += *it;
  }

  BOOST_TEST(result == "47 12");
}

BOOST_AUTO_TEST_CASE(bitwiseOrPriority)
{
  const std::string input = "1 + 2 | 4 * 2\n";
  std::istringstream stream(input);
  yalovsky::Stack< yalovsky::Queue< std::string > > infix;

  yalovsky::getInfix(stream, infix);

  yalovsky::Queue< std::string > postfix;
  yalovsky::convertInfToPost(infix.drop(), postfix);

  BOOST_TEST(yalovsky::calculate(postfix) == "11");
}
