#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <stdexcept>

#include "ExpressionProcess.hpp"

BOOST_AUTO_TEST_SUITE(expression_read_suite)

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

BOOST_AUTO_TEST_SUITE(input_stream_reading_suite)

using namespace khasnulin;

BOOST_AUTO_TEST_CASE(test_expressions_order_inversion)
{
  std::stringstream ss("1 + 1\n2 + 2\n3 + 3");

  Stack< Queue< Token > > allExprs = readAllExpressions(ss);

  BOOST_REQUIRE_EQUAL(allExprs.size(), 3);

  Queue< Token > lastExpr = allExprs.top();
  BOOST_CHECK_EQUAL(lastExpr.front().value, 3);
  allExprs.pop();

  Queue< Token > middleExpr = allExprs.top();
  BOOST_CHECK_EQUAL(middleExpr.front().value, 2);
  allExprs.pop();

  Queue< Token > firstExpr = allExprs.top();
  BOOST_CHECK_EQUAL(firstExpr.front().value, 1);
}

BOOST_AUTO_TEST_CASE(test_skip_empty_lines)
{
  std::stringstream ss("1 + 1\n\n2 + 2");
  Stack< Queue< Token > > allExprs = readAllExpressions(ss);

  BOOST_CHECK_EQUAL(allExprs.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_eof_handling)
{
  std::stringstream ss("1 + 1\n");
  Stack< Queue< Token > > allExprs = readAllExpressions(ss);

  BOOST_CHECK_EQUAL(allExprs.size(), 1);
  BOOST_CHECK(!allExprs.empty());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(process_token_suite)

using namespace khasnulin;

BOOST_AUTO_TEST_CASE(test_step_by_step_stacks)
{
  Stack< Token > values;
  Stack< Token > operations;

  Token t1 = {TokenType::Number, 3, '0', 0};
  processToken(t1, values, operations);

  BOOST_CHECK_EQUAL(values.size(), 1);
  BOOST_CHECK_EQUAL(values.top().value, 3);
  BOOST_CHECK(operations.empty());

  Token t2 = {TokenType::Operator, 0, '+', 1};
  processToken(t2, values, operations);

  BOOST_CHECK_EQUAL(values.size(), 1);
  BOOST_CHECK_EQUAL(operations.size(), 1);
  BOOST_CHECK_EQUAL(operations.top().op, '+');

  Token t3 = {TokenType::Number, 4, '0', 0};
  processToken(t3, values, operations);

  BOOST_CHECK_EQUAL(values.size(), 2);
  BOOST_CHECK_EQUAL(values.top().value, 4);
  BOOST_CHECK_EQUAL(operations.top().op, '+');
}

BOOST_AUTO_TEST_CASE(test_operator_priority)
{
  Stack< Token > values;
  Stack< Token > operations;

  values.push({TokenType::Number, 10, '0', 0});
  values.push({TokenType::Number, 5, '0', 0});
  operations.push({TokenType::Operator, 0, '*', 2});

  Token plus = {TokenType::Operator, 0, '+', 1};

  processToken(plus, values, operations);

  BOOST_CHECK_EQUAL(operations.top().op, '+');
  BOOST_CHECK_EQUAL(values.top().value, 50);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(expression_processing_suite)
using namespace khasnulin;

BOOST_AUTO_TEST_CASE(test_basic_operations)
{
  BOOST_CHECK_EQUAL(readAndProcessExpressionLine("2 + 2"), 4);
  BOOST_CHECK_EQUAL(readAndProcessExpressionLine("10 - 3"), 7);
  BOOST_CHECK_EQUAL(readAndProcessExpressionLine("5 * 4"), 20);
  BOOST_CHECK_EQUAL(readAndProcessExpressionLine("20 / 5"), 4);
}

BOOST_AUTO_TEST_CASE(test_operator_precedence)
{
  BOOST_CHECK_EQUAL(readAndProcessExpressionLine("2 + 3 * 4"), 14);
  BOOST_CHECK_EQUAL(readAndProcessExpressionLine("15 - 10 / 2"), 10);
}

BOOST_AUTO_TEST_CASE(test_parentheses)
{
  BOOST_CHECK_EQUAL(readAndProcessExpressionLine("( 2 + 3 ) * 4"), 20);
  BOOST_CHECK_EQUAL(readAndProcessExpressionLine("2 * ( 3 + ( 10 / 2 ) )"), 16);
}

BOOST_AUTO_TEST_CASE(test_minimal_expressions)
{
  BOOST_CHECK_EQUAL(readAndProcessExpressionLine("42"), 42);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(expression_processing_with_overflow_suite)
using namespace khasnulin;

BOOST_AUTO_TEST_CASE(test_addition_overflow)
{
  std::string expr = std::to_string(std::numeric_limits< int >::max()) + " + 1";
  BOOST_CHECK_THROW(readAndProcessExpressionLine(expr), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(test_addition_underflow)
{
  std::string expr = "( 0 - " + (std::to_string(std::numeric_limits< int >::max())) + " - 1 ) + ( 0 - 1 )";
  BOOST_CHECK_THROW(readAndProcessExpressionLine(expr), std::underflow_error);
}

BOOST_AUTO_TEST_CASE(test_subtraction_underflow)
{
  std::string expr = "( 0 - " + (std::to_string(std::numeric_limits< int >::max())) + " - 1 ) - 1";
  BOOST_CHECK_THROW(readAndProcessExpressionLine(expr), std::underflow_error);
}

BOOST_AUTO_TEST_CASE(test_subtraction_overflow_with_negative)
{
  std::string expr = std::to_string(std::numeric_limits< int >::max()) + " - ( 0 - 1 )";
  BOOST_CHECK_THROW(readAndProcessExpressionLine(expr), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(test_division_by_zero)
{
  BOOST_CHECK_THROW(readAndProcessExpressionLine("10 / 0"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(test_division_int_min_by_minus_one)
{
  std::string min_int_str = "( 0 - " + (std::to_string(std::numeric_limits< int >::max())) + " - 1 )";
  std::string expr = min_int_str + " / ( 0 - 1 )";
  BOOST_CHECK_THROW(readAndProcessExpressionLine(expr), std::underflow_error);
}

BOOST_AUTO_TEST_CASE(test_multiplication_overflow)
{
  std::string expr = std::to_string(std::numeric_limits< int >::max() / 2 + 1) + " * 2";
  BOOST_CHECK_THROW(readAndProcessExpressionLine(expr), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(test_multiplication_overflow_binary)
{
  std::string expr = "( 0 - 100000 ) * ( 0 - 30000 )";
  BOOST_CHECK_THROW(readAndProcessExpressionLine(expr), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(test_multiplication_underflow)
{
  std::string expr = std::to_string(std::numeric_limits< int >::max() / 2 + 1) + " * ( 0 - 4 ) ";
  BOOST_CHECK_THROW(readAndProcessExpressionLine(expr), std::underflow_error);
}

BOOST_AUTO_TEST_CASE(test_modulo_overflow)
{
  std::string expr = "( 0 - " + std::to_string(std::numeric_limits< int >::max()) + " - 1 ) % ( 0 - 1 )";
  BOOST_CHECK_THROW(readAndProcessExpressionLine(expr), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(test_modulo_by_zero)
{
  BOOST_CHECK_THROW(readAndProcessExpressionLine("10 % 0"), std::logic_error);
}

BOOST_AUTO_TEST_SUITE_END()
