#include <boost/test/unit_test.hpp>

#include "func-stack-queue.hpp"
#include "func-math.hpp"
#include "stack.hpp"

BOOST_AUTO_TEST_SUITE(operation_test)

BOOST_AUTO_TEST_CASE(BitShiftToRightTest)
{

  std::string inp = "( 10 + 5 ) ## 3\n100 ## ( 2 * 3 )\n( 2 ## 3 ) * ( 4 ## 5 )";
  std::istringstream input(inp);

  std::ostringstream output;

  zubarev::Stack< long long > results;
  std::string expression = "";
  while (std::getline(input, expression)) {
    zubarev::Queue< std::string > infixQ = zubarev::detail::fromStrToQueue(expression);
    if (infixQ.empty()) {
      continue;
    }

    zubarev::Queue< std::string > postfixQ = zubarev::detail::fromInfixToPostfix(infixQ);
    results.push(zubarev::eval(postfixQ));
  }

  if (results.empty()) {
    output << '\n';
    return;
  }
  output << results.top();
  results.drop();
  while (!results.empty()) {
    output << ' ' << results.top();
    results.drop();
  }

  output << '\n';

  BOOST_CHECK(output.str() == "1035 1006 153\n");
}

BOOST_AUTO_TEST_SUITE_END()
