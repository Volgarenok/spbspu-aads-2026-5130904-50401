#include <boost/test/unit_test.hpp>
#include "math.hpp"

BOOST_AUTO_TEST_SUITE(gcdTest)

BOOST_AUTO_TEST_CASE(gcd_positive_numbers)
{
  BOOST_CHECK(gcd(12, 18) == 6);
  BOOST_CHECK(gcd(0, 5) == 5);
  BOOST_CHECK(gcd(5, 0) == 5);
  BOOST_CHECK(gcd(1, 100) == 1);
}

BOOST_AUTO_TEST_CASE(gcd_negative_numbers)
{
  BOOST_CHECK(gcd(-12, 18) == 6);
  BOOST_CHECK(gcd(-12, -18) == 6);
  BOOST_CHECK(gcd(17, -19) == 1);
}

BOOST_AUTO_TEST_CASE(gcd_equal_numbers)
{
  BOOST_CHECK(gcd(7, 7) == 7);
  BOOST_CHECK(gcd(100, 100) == 100);
  BOOST_CHECK(gcd(0, 0) == 0);
}

BOOST_AUTO_TEST_SUITE_END()
