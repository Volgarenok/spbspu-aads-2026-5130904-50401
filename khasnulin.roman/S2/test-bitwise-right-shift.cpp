#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <stdexcept>

#include "ExpressionProcess.hpp"

BOOST_AUTO_TEST_SUITE(bitwise_right_shift_suite)

using namespace khasnulin;

BOOST_AUTO_TEST_CASE(test_right_shift_basic)
{
  BOOST_CHECK_EQUAL(readAndProcessExpressionLine("16 >> 2"), 4);
}

BOOST_AUTO_TEST_CASE(test_negative_shift_count)
{
  BOOST_CHECK_THROW(readAndProcessExpressionLine("16 >> ( 0 - 1 )"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(test_large_shift_count)
{
  BOOST_CHECK_EQUAL(readAndProcessExpressionLine("100 >> 40"), 0);
}

BOOST_AUTO_TEST_CASE(test_large_shift_count_negative)
{
  BOOST_CHECK_EQUAL(readAndProcessExpressionLine("( 0 - 5 ) >> 40"), -1);
}

BOOST_AUTO_TEST_SUITE_END()
