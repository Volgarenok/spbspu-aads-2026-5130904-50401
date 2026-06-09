#include <boost/test/unit_test.hpp>
#include "math_funcs.hpp"

BOOST_AUTO_TEST_SUITE(lcm_tests)

BOOST_AUTO_TEST_CASE(test_basic)
{
  BOOST_CHECK_EQUAL(chernov::lcm(4, 6), 12);
  BOOST_CHECK_EQUAL(chernov::lcm(6, 4), 12);
  BOOST_CHECK_EQUAL(chernov::lcm(3, 5), 15);
  BOOST_CHECK_EQUAL(chernov::lcm(12, 18), 36);
  BOOST_CHECK_EQUAL(chernov::lcm(1, 100), 100);
  BOOST_CHECK_EQUAL(chernov::lcm(7, 7), 7);
}

BOOST_AUTO_TEST_CASE(test_zero)
{
  BOOST_CHECK_EQUAL(chernov::lcm(0, 5), 0);
  BOOST_CHECK_EQUAL(chernov::lcm(5, 0), 0);
  BOOST_CHECK_EQUAL(chernov::lcm(0, 0), 0);
}

BOOST_AUTO_TEST_CASE(test_negative)
{
  BOOST_CHECK_EQUAL(chernov::lcm(-4, 6), 12);
  BOOST_CHECK_EQUAL(chernov::lcm(4, -6), 12);
  BOOST_CHECK_EQUAL(chernov::lcm(-4, -6), 12);
}

BOOST_AUTO_TEST_CASE(test_mutually_prime)
{
  BOOST_CHECK_EQUAL(chernov::lcm(17, 19), 323);
}

BOOST_AUTO_TEST_CASE(test_multiple)
{
  BOOST_CHECK_EQUAL(chernov::lcm(8, 4), 8);
  BOOST_CHECK_EQUAL(chernov::lcm(9, 3), 9);
}

BOOST_AUTO_TEST_CASE(test_overflow)
{
  BOOST_CHECK_THROW(chernov::lcm(chernov::MAX_LLI / 2 + 2, 2), std::overflow_error);
  BOOST_CHECK_THROW(chernov::lcm(chernov::MAX_LLI / 3 + 2, 3), std::overflow_error);
  BOOST_CHECK_THROW(chernov::lcm(chernov::MAX_LLI / 1000000 + 3, 1000000), std::overflow_error);
  BOOST_CHECK_THROW(chernov::lcm(chernov::MAX_LLI / 2 + 1, 3), std::overflow_error);
}

BOOST_AUTO_TEST_SUITE_END()
