#include <boost/test/unit_test.hpp>
#include "math_functions.hpp"

BOOST_AUTO_TEST_CASE(Concatenation_test)
{
  std::string inputData = "( 10 + 1 ) ## 21\n11 ## 20\n10 ## 4 ## 2\n";
  std::istringstream iss(inputData);
  malashenko::Stack< malashenko::Queue< std::string > > infixAllData;
  getInfixData(iss, infixAllData);
  malashenko::List< std::string > out;

  while (!infixAllData.empty())
  {
    malashenko::Queue< std::string > infixData = infixAllData.top();
    infixAllData.pop();
    malashenko::Queue< std::string > postfixData;
    converInfixToPostfix(infixData, postfixData);
    std::string res;
    res = malashenko::calculate(postfixData);
    out.push_back(res);
  }

  std::string resStr;
  malashenko::LIter< std::string > it = out.begin();
  resStr += *it;
  ++it;

  for (; it != out.end(); ++it)
  {
    resStr += ' ';
    resStr += *it;
  }
  BOOST_CHECK(resStr == "1042 1120 1121");
}
