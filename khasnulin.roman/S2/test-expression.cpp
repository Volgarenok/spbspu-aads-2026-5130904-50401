#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>

#include "ExpressionProcess.hpp"

BOOST_AUTO_TEST_SUITE(stack_test_suite)

using namespace khasnulin;

BOOST_AUTO_TEST_CASE(test_read_full_expression)
{
  std::stringstream ss("10 + 20 * ( 3 - 1 )");
  Queue< Token > q = readExpression(ss);

  BOOST_CHECK_EQUAL(q.size(), 9);

  BOOST_CHECK_EQUAL(q.front().value, 10);

  BOOST_CHECK_EQUAL(q.back().type, TokenType::RightParen);
}

BOOST_AUTO_TEST_CASE(test_read_empty_expression)
{
  std::stringstream ss("\n");
  Queue< Token > q = readExpression(ss);

  BOOST_REQUIRE_EQUAL(q.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_read_expression_with_spaces)
{
  std::stringstream ss("   42   +   7   ");
  Queue< Token > q = readExpression(ss);

  BOOST_REQUIRE_EQUAL(q.size(), 3);
  BOOST_CHECK_EQUAL(q.front().value, 42);
}

BOOST_AUTO_TEST_CASE(test_read_only_one_line)
{
  std::stringstream ss("1 + 2\n3 + 4");
  Queue< Token > q = readExpression(ss);

  BOOST_CHECK_EQUAL(q.size(), 3);
  BOOST_CHECK_EQUAL(q.front().value, 1);
  BOOST_CHECK_EQUAL(q.back().value, 2);

  std::string remaining;
  std::getline(ss, remaining);
  BOOST_CHECK_EQUAL(remaining, "3 + 4");
}

BOOST_AUTO_TEST_SUITE_END()
