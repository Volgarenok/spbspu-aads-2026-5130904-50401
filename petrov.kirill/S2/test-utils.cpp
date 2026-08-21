#include <boost/test/unit_test.hpp>
#include <sstream>
#include "Stack.hpp"
#include "utils.hpp"

BOOST_AUTO_TEST_SUITE(UtilsTests)

BOOST_AUTO_TEST_CASE(prior_check)
{
  BOOST_CHECK_EQUAL(petrov::detail::prior('+'), 2);
  BOOST_CHECK_EQUAL(petrov::detail::prior('-'), 2);
  BOOST_CHECK_EQUAL(petrov::detail::prior('*'), 3);
  BOOST_CHECK_EQUAL(petrov::detail::prior('/'), 3);
  BOOST_CHECK_EQUAL(petrov::detail::prior('%'), 3);
  BOOST_CHECK_EQUAL(petrov::detail::prior('&'), 1);
  BOOST_CHECK_EQUAL(petrov::detail::prior('('), 0);
}

BOOST_AUTO_TEST_CASE(oper_base)
{
  BOOST_CHECK_EQUAL(petrov::detail::oper(10, 5, '+'), 15);
  BOOST_CHECK_EQUAL(petrov::detail::oper(10, 5, '-'), 5);
  BOOST_CHECK_EQUAL(petrov::detail::oper(10, 5, '*'), 50);
  BOOST_CHECK_EQUAL(petrov::detail::oper(10, 5, '/'), 2);
  BOOST_CHECK_EQUAL(petrov::detail::oper(10, 3, '%'), 1);
  BOOST_CHECK_EQUAL(petrov::detail::oper(10, 3, '&'), 2);
}

BOOST_AUTO_TEST_CASE(calc_simple_line)
{
  std::stringstream strs;
  strs << "1 + 2 * 3\n";

  petrov::Stack<long long int> res = petrov::calcStream(strs);

  BOOST_CHECK_EQUAL(res.size(), 1);
  BOOST_CHECK_EQUAL(res.top(), 7);
}

BOOST_AUTO_TEST_CASE(calc_multi_lines)
{
  std::stringstream strs;
  strs << "10 + 20\n";
  strs << "( 30 - 10 ) / 2\n";

  petrov::Stack<long long int> res = petrov::calcStream(strs);

  BOOST_CHECK_EQUAL(res.size(), 2);
  BOOST_CHECK_EQUAL(res.drop(), 10);
  BOOST_CHECK_EQUAL(res.drop(), 30);
}

BOOST_AUTO_TEST_CASE(calc_bitwise_and)
{
  std::stringstream strs;
  strs << "12 & 10\n";

  petrov::Stack<long long int> res = petrov::calcStream(strs);

  BOOST_CHECK_EQUAL(res.size(), 1);
  BOOST_CHECK_EQUAL(res.top(), 8);
}

BOOST_AUTO_TEST_SUITE_END()
