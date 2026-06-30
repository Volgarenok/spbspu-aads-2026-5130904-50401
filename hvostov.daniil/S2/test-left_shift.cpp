#include <boost/test/unit_test.hpp>
#include "arifmetic_operations.hpp"

BOOST_AUTO_TEST_SUITE(shift_suite)

BOOST_AUTO_TEST_CASE(test_common_situation)
{
  BOOST_CHECK_EQUAL(hvostov::shiftLeftWithOverflowCheck(2, 2), 8);
  BOOST_CHECK_EQUAL(hvostov::shiftLeftWithOverflowCheck(3, 2), 12);
  BOOST_CHECK_EQUAL(hvostov::shiftLeftWithOverflowCheck(3, 1), 6);
  BOOST_CHECK_EQUAL(hvostov::shiftLeftWithOverflowCheck(7, 2), 28);
  BOOST_CHECK_EQUAL(hvostov::shiftLeftWithOverflowCheck(1, 0), 1);
  BOOST_CHECK_EQUAL(hvostov::shiftLeftWithOverflowCheck(0, 5), 0);
  BOOST_CHECK_EQUAL(hvostov::shiftLeftWithOverflowCheck(-2, 2), -8);
  BOOST_CHECK_EQUAL(hvostov::shiftLeftWithOverflowCheck(-3, 2), -12);
}

BOOST_AUTO_TEST_CASE(test_shift_by_zero)
{
  BOOST_CHECK_EQUAL(hvostov::shiftLeftWithOverflowCheck(42, 0), 42);
  BOOST_CHECK_EQUAL(hvostov::shiftLeftWithOverflowCheck(-42, 0), -42);
  BOOST_CHECK_EQUAL(hvostov::shiftLeftWithOverflowCheck(0, 0), 0);
}

BOOST_AUTO_TEST_CASE(test_negative_shift_throws)
{
  BOOST_CHECK_THROW(hvostov::shiftLeftWithOverflowCheck(5, -1), std::logic_error);
  BOOST_CHECK_THROW(hvostov::shiftLeftWithOverflowCheck(10, -5), std::logic_error);
  BOOST_CHECK_THROW(hvostov::shiftLeftWithOverflowCheck(0, -1), std::logic_error);
}

BOOST_AUTO_TEST_CASE(test_shift_exceeds_bit_width_throws)
{
  int bitWidth = sizeof(long long int) * 8;
  BOOST_CHECK_THROW(hvostov::shiftLeftWithOverflowCheck(1, bitWidth), std::overflow_error);
  BOOST_CHECK_THROW(hvostov::shiftLeftWithOverflowCheck(1, bitWidth + 1), std::overflow_error);
  BOOST_CHECK_THROW(hvostov::shiftLeftWithOverflowCheck(100, bitWidth), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(test_overflow_throws)
{
  BOOST_CHECK_THROW(hvostov::shiftLeftWithOverflowCheck(hvostov::LLMAX, 1), std::overflow_error);
  BOOST_CHECK_THROW(hvostov::shiftLeftWithOverflowCheck(hvostov::LLMAX, 2), std::overflow_error);
  BOOST_CHECK_THROW(hvostov::shiftLeftWithOverflowCheck((hvostov::LLMAX >> 1) + 1, 1), std::overflow_error);
  BOOST_CHECK_THROW(hvostov::shiftLeftWithOverflowCheck((hvostov::LLMAX >> 2) + 2, 2), std::overflow_error);
  BOOST_CHECK_THROW(hvostov::shiftLeftWithOverflowCheck(hvostov::LLMIN, 1), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(test_max_safe_shifts)
{
  int bitWidth = sizeof(long long int) * 8;
  BOOST_CHECK_NO_THROW(hvostov::shiftLeftWithOverflowCheck(1, bitWidth - 2));
  long long int maxSafe = hvostov::LLMAX >> 1;
  BOOST_CHECK_NO_THROW(hvostov::shiftLeftWithOverflowCheck(maxSafe, 1));
  BOOST_CHECK_EQUAL(hvostov::shiftLeftWithOverflowCheck(maxSafe, 1), hvostov::LLMAX - 1);
  BOOST_CHECK_NO_THROW(hvostov::shiftLeftWithOverflowCheck(hvostov::LLMAX >> 2, 2));
  BOOST_CHECK_NO_THROW(hvostov::shiftLeftWithOverflowCheck(hvostov::LLMAX >> 3, 3));
}

BOOST_AUTO_TEST_CASE(test_shift_with_negative_numbers)
{
  BOOST_CHECK_EQUAL(hvostov::shiftLeftWithOverflowCheck(-2, 2), -8);
  BOOST_CHECK_EQUAL(hvostov::shiftLeftWithOverflowCheck(-1, 1), -2);
  BOOST_CHECK_EQUAL(hvostov::shiftLeftWithOverflowCheck(-5, 2), -20);
  BOOST_CHECK_EQUAL(hvostov::shiftLeftWithOverflowCheck(-7, 3), -56);
}

BOOST_AUTO_TEST_CASE(test_large_shift_amount)
{
  int bitWidth = sizeof(long long int) * 8;
  BOOST_CHECK_NO_THROW(hvostov::shiftLeftWithOverflowCheck(-1, bitWidth - 1));
  BOOST_CHECK_EQUAL(hvostov::shiftLeftWithOverflowCheck(-1, bitWidth - 1), -(1LL << (bitWidth - 1)));
}

BOOST_AUTO_TEST_CASE(test_zero_left_operand)
{
  BOOST_CHECK_EQUAL(hvostov::shiftLeftWithOverflowCheck(0, 1), 0);
  BOOST_CHECK_EQUAL(hvostov::shiftLeftWithOverflowCheck(0, 10), 0);
  BOOST_CHECK_EQUAL(hvostov::shiftLeftWithOverflowCheck(0, 63), 0);
}

BOOST_AUTO_TEST_SUITE_END()
