#include <boost/test/unit_test.hpp>
#include "math-operations.hpp"

BOOST_AUTO_TEST_SUITE(operationsTest)
BOOST_AUTO_TEST_CASE(addition)
{
  donkeev::llint_t val1 = 2;
  donkeev::llint_t val2 = 3;
  BOOST_TEST(donkeev::doOperation(val1, val2, '+') == 5);
}

BOOST_AUTO_TEST_CASE(subtraction)
{
  donkeev::llint_t val1 = 3;
  donkeev::llint_t val2 = 2;
  BOOST_TEST(donkeev::doOperation(val1, val2, '-') == 1);
}

BOOST_AUTO_TEST_CASE(multiplication)
{
  donkeev::llint_t val1 = 3;
  donkeev::llint_t val2 = 2;
  BOOST_TEST(donkeev::doOperation(val1, val2, '*') == 6);
}

BOOST_AUTO_TEST_CASE(division)
{
  donkeev::llint_t val1 = 3;
  donkeev::llint_t val2 = 2;
  BOOST_TEST(donkeev::doOperation(val1, val2, '/') == 1);
}

BOOST_AUTO_TEST_CASE(modulo)
{
  donkeev::llint_t val1 = 3;
  donkeev::llint_t val2 = 2;
  BOOST_TEST(donkeev::doOperation(val1, val2, '%') == 1);
}

BOOST_AUTO_TEST_CASE(bitwiseAnd)
{
  donkeev::llint_t val1 = 3;
  donkeev::llint_t val2 = 2;
  BOOST_TEST(donkeev::doOperation(val1, val2, '&') == 2);
}

BOOST_AUTO_TEST_SUITE_END()
