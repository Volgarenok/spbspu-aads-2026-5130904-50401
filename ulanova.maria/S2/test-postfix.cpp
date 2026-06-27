#include "postfix.hpp"
#include "boost/test/unit_test.hpp"

BOOST_AUTO_TEST_CASE(test_simple_postfix)
{
  BOOST_TEST_CHECK(ulanova::postfix("2 3 +") == 5);
  BOOST_TEST_CHECK(ulanova::postfix("4 3 +") == 7);
  BOOST_TEST_CHECK(ulanova::postfix("10 2 /") == 5);
}
BOOST_AUTO_TEST_CASE(test_complex_postfix)
{
  BOOST_TEST_CHECK(ulanova::postfix("2 3 + 4 *") == 20);
}
BOOST_AUTO_TEST_CASE(test_xor_postfix)
{
  BOOST_TEST_CHECK(ulanova::postfix("5 3 ^") == 6);
}
