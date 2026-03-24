#include <boost/test/unit_test.hpp>
#include <iostream>
#include "../common/includes/stack.hpp"
#include "../common/includes/queue.hpp"
#include "math_funcs.hpp"


BOOST_AUTO_TEST_CASE(BitShiftToRightTest)
{
  namespace kuz = kuznetsov;
  std::string inp = "( 125 >> 2 ) + 9 * 3 - ( 100 >> 3 ) / 4\n64 >> 2\n( 15 + 5 ) / ( 28 / 7 )\n";
  std::istringstream input(inp);

  kuz::Stack< kuz::Queue< std::string > > mathExpressions;
  kuz::getExpressions(input, mathExpressions);

  kuz::Queue< kuz::lli_t > res;
  kuz::calculate(mathExpressions, res);
  std::string result = "";
  result += std::to_string(res.front());
  res.pop();
  result += ' ';
  result += std::to_string(res.front());
  res.pop();
  result += ' ';
  result += std::to_string(res.front());
  res.pop();
  BOOST_CHECK(result == "5 16 55");
}



