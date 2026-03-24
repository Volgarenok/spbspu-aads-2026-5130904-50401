#include "math_functions.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_operations)
{
  BOOST_TEST_CHECK(ulanova::apply_op(2, 3, '+') == 5);
  BOOST_TEST_CHECK(ulanova::apply_op(5, 3, '-') == 2);
  BOOST_TEST_CHECK(ulanova::apply_op(4, 3, '*') == 12);
  BOOST_TEST_CHECK(ulanova::apply_op(6, 3, '/') == 2);
  BOOST_TEST_CHECK(ulanova::apply_op(7, 3, '%') == 1);
}
BOOST_AUTO_TEST_CASE(test_xor)
{
  BOOST_TEST_CHECK(ulanova::apply_op(5, 3, '^') == 6);
  BOOST_TEST_CHECK(ulanova::apply_op(6, 3, '^') == 5);
}
BOOST_AUTO_TEST_CASE(test_priority)
{
  BOOST_TEST_CHECK(ulanova::priority('+') == 1);
  BOOST_TEST_CHECK(ulanova::priority('*') == 2);
  BOOST_TEST_CHECK(ulanova::priority('^') == 1);
}
BOOST_AUTO_TEST_CASE(test_is_operator)
{
  BOOST_TEST_CHECK(ulanova::is_operator('+'));
  BOOST_TEST_CHECK(ulanova::is_operator('^'));
  BOOST_TEST_CHECK(!ulanova::is_operator('u'));
}
BOOST_AUTO_TEST_CASE(test_div_zero)
{
  BOOST_CHECK_THROW(ulanova::apply_op(1, 0, '/'), std::runtime_error);
}
