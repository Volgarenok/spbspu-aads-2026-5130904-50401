#include "infix.hpp"
#include "boost/test/unit_test.hpp"

BOOST_AUTO_TEST_CASE(test_priority_infix)
{
  BOOST_TEST_CHECK(ulanova::infix_to_postfix("2 + 3 * 4") == "2 3 4 * + ");
}
