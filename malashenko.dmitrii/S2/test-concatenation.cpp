#include <boost/test/unit_test.hpp>
#include "math_functions.hpp"
#include "calculator.hpp"

BOOST_AUTO_TEST_CASE(Concatenation_test)
{
  malashenko::Calculator calc;
  std::string inputData = "( 10 + 1 ) ## 21\n11 ## 20\n10 ## 4 ## 2\n";
  std::istringstream iss(inputData);
  calc.getInfixData(iss);

  while (!calc.isInfixAllDataEmpty())
  {
    calc.convertInfixToPostfix();
    calc.calculateFromPostfix();
    calc.pushToOut();
  }

  std::string resStr = calc.getOutList();
  BOOST_CHECK_EQUAL(resStr,"1042 1120 1121");
}
