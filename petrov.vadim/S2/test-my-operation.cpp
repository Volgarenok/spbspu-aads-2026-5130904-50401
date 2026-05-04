#include <boost/test/unit_test.hpp>
#include <sstream>
#include <string>
#include "foo.hpp"
#include "stack.hpp"
#include "math.hpp"

BOOST_AUTO_TEST_SUITE(Reverse_operation_test)

BOOST_AUTO_TEST_CASE(ReverseNumber_basic_test)
{
  std::string inputData = "# 123\n12 + # 34\n# # 100\n# -456\n# 0\n";
  std::istringstream iss(inputData);

  petrov::Stack< petrov::Queue< std::string > > infix;
  petrov::Stack< petrov::Queue< std::string > > postfix;
  petrov::Stack< long long > results;

  petrov::getInfix(iss, infix);
  petrov::infixToPostfix(infix, postfix);

  while (!postfix.empty())
  {
    petrov::Queue< std::string > expr = postfix.drop();
    long long value = petrov::calculatePostfix(expr);
    results.push(value);
  }

  std::string resStr;
  bool first = true;
  while (!results.empty())
  {
    if (!first)
    {
      resStr = " " + resStr;
    }
    first = false;
    resStr = std::to_string(results.drop()) + resStr;
  }

  BOOST_CHECK_EQUAL(resStr, "321 55 1 -654 0");
}

BOOST_AUTO_TEST_SUITE_END()
