#include <boost/test/unit_test.hpp>
#include "math_functions.hpp"

using ll_t = zharov::ll_t;

BOOST_AUTO_TEST_CASE(ShiftNormalValuesTest)
{
  BOOST_CHECK_EQUAL(zharov::bitShiftLeft(1, 1), 2);
  BOOST_CHECK_EQUAL(zharov::bitShiftLeft(1, 2), 4);
  BOOST_CHECK_EQUAL(zharov::bitShiftLeft(5, 1), 10);
  BOOST_CHECK_EQUAL(zharov::bitShiftLeft(5, 2), 20);
  BOOST_CHECK_EQUAL(zharov::bitShiftLeft(10, 3), 80);
}

BOOST_AUTO_TEST_CASE(ShiftNegativeNumberTest)
{
  BOOST_CHECK_THROW(zharov::bitShiftLeft(-1, 1), std::overflow_error);
  BOOST_CHECK_THROW(zharov::bitShiftLeft(-5, 2), std::overflow_error);
  BOOST_CHECK_THROW(zharov::bitShiftLeft(std::numeric_limits< ll_t >::min(), 1),
    std::overflow_error);
}

BOOST_AUTO_TEST_CASE(ShiftNegativeShiftAmountTest)
{
  BOOST_CHECK_THROW(zharov::bitShiftLeft(5, -1), std::overflow_error);
  BOOST_CHECK_THROW(zharov::bitShiftLeft(10, -5), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(ShiftTooLargeShiftAmountTest)
{
  int width = sizeof(ll_t) * CHAR_BIT;
  BOOST_CHECK_THROW(zharov::bitShiftLeft(1, width), std::overflow_error);
  BOOST_CHECK_THROW(zharov::bitShiftLeft(1, width + 1), std::overflow_error);
  BOOST_CHECK_THROW(zharov::bitShiftLeft(1, 100), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(ShiftOverflowTest)
{
  ll_t max = std::numeric_limits< ll_t >::max();
  BOOST_CHECK_THROW(zharov::bitShiftLeft((max >> 1) + 1, 1), std::overflow_error);
  BOOST_CHECK_THROW(zharov::bitShiftLeft((max >> 2) + 1, 2), std::overflow_error);
  BOOST_CHECK_THROW(zharov::bitShiftLeft(max, 1), std::overflow_error);
}
