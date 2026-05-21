#include "all.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(operation_test)

BOOST_AUTO_TEST_CASE(BitShiftToRightTest)
{

  std::string inp = "( 10 + 5 ) ## 3\n100 ## ( 2 * 3 )\n( 2 ## 3 ) * ( 4 ## 5 )";
  std::istringstream inputFlow(inp);
  std::string resString = "";
  while (inputFlow && !inputFlow.eof()) {
    bool error = false;
    std::string expression = zubarev::input(inputFlow, error);

    if (error) {
      throw std::runtime_error("input: incorrect input");
    }

    if (!expression.empty()) {
      zubarev::Queue< std::string > infixQ = zubarev::fromStrToQueue(expression);
      zubarev::Queue< std::string > postfixQ = zubarev::fromInfixToPostfix(infixQ);
      resString += std::to_string(zubarev::eval(postfixQ));
      resString += " ";
    }
  }

  BOOST_CHECK(resString == "153 1006 1035 ");
}

BOOST_AUTO_TEST_SUITE_END()
