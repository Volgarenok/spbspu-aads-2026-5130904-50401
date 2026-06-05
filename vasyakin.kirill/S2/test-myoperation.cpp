#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include "math_op.hpp"

BOOST_AUTO_TEST_SUITE(MathLcmTests)

BOOST_AUTO_TEST_CASE(lcm_basic_positive)
{
  BOOST_CHECK_EQUAL(vasyakin::lcm(4, 6), 12);
  BOOST_CHECK_EQUAL(vasyakin::lcm(3, 5), 15);
  BOOST_CHECK_EQUAL(vasyakin::lcm(6, 8), 24);
}

BOOST_AUTO_TEST_CASE(lcm_with_one)
{
  BOOST_CHECK_EQUAL(vasyakin::lcm(1, 5), 5);
  BOOST_CHECK_EQUAL(vasyakin::lcm(7, 1), 7);
}

BOOST_AUTO_TEST_CASE(lcm_same_numbers)
{
  BOOST_CHECK_EQUAL(vasyakin::lcm(5, 5), 5);
  BOOST_CHECK_EQUAL(vasyakin::lcm(10, 10), 10);
}

BOOST_AUTO_TEST_CASE(lcm_negative_numbers)
{
  BOOST_CHECK_EQUAL(vasyakin::lcm(-4, 6), 12);
  BOOST_CHECK_EQUAL(vasyakin::lcm(4, -6), 12);
  BOOST_CHECK_EQUAL(vasyakin::lcm(-4, -6), 12);
}

BOOST_AUTO_TEST_CASE(lcm_zero_throws)
{
  BOOST_CHECK_THROW(vasyakin::lcm(0, 5), std::runtime_error);
  BOOST_CHECK_THROW(vasyakin::lcm(5, 0), std::runtime_error);
  BOOST_CHECK_THROW(vasyakin::lcm(0, 0), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()
