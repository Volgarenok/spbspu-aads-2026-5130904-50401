#include <boost/test/unit_test.hpp>
#include "math-operations.hpp"
using llint_t = long long int;

BOOST_AUTO_TEST_SUITE(operationsTest)
BOOST_AUTO_TEST_CASE(addition)
{
  llint_t val1 = 2;
  llint_t val2 = 3;
  BOOST_TEST(donkeev::doOperation(val1, val2, '+') == 5);
}

BOOST_AUTO_TEST_CASE(subtraction)
{
  llint_t val1 = 3;
  llint_t val2 = 2;
  BOOST_TEST(donkeev::doOperation(val1, val2, '-') == 1);
}

BOOST_AUTO_TEST_CASE(multiplication)
{
  llint_t val1 = 3;
  llint_t val2 = 2;
  BOOST_TEST(donkeev::doOperation(val1, val2, '*') == 6);
}

BOOST_AUTO_TEST_CASE(division)
{
  llint_t val1 = 3;
  llint_t val2 = 2;
  BOOST_TEST(donkeev::doOperation(val1, val2, '/') == 1);
}

BOOST_AUTO_TEST_CASE(modulo)
{
  llint_t val1 = 3;
  llint_t val2 = 2;
  BOOST_TEST(donkeev::doOperation(val1, val2, '%') == 1);
}

BOOST_AUTO_TEST_CASE(bitwiseAnd)
{
  llint_t val1 = 3;
  llint_t val2 = 2;
  BOOST_TEST(donkeev::doOperation(val1, val2, '&') == 2);
}

BOOST_AUTO_TEST_SUITE_END()
