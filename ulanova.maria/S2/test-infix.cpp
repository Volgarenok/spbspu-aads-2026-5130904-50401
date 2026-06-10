#include "infix.hpp"
#include "boost/test/unit_test.hpp"

BOOST_AUTO_TEST_CASE(test_priority_infix)
{
  ulanova::Queue< std::string > result = ulanova::infix_to_postfix("2 + 3 * 4");

  BOOST_TEST_CHECK(result.front() == "2");
  result.pop();

  BOOST_TEST_CHECK(result.front() == "3");
  result.pop();

  BOOST_TEST_CHECK(result.front() == "4");
  result.pop();

  BOOST_TEST_CHECK(result.front() == "*");
  result.pop();

  BOOST_TEST_CHECK(result.front() == "+");
  result.pop();

  BOOST_TEST_CHECK(result.empty());
}
