#include "boost/test/unit_test.hpp"
#include "functions.hpp"

using namespace tarasenko;

BOOST_AUTO_TEST_CASE(get_number)
{
  std::string s = "12345";
  size_t pos = 0;
  BOOST_TEST(getNumber(s, pos) == 12345);
  BOOST_TEST(pos == 5);
  s = "12345a";
  pos = 1;
  BOOST_TEST(getNumber(s, pos) == 2345);
  BOOST_TEST(pos == 5);
}

void checkToken(const Token& token, TokenType type, long long value, char operation, int priority)
{
  BOOST_CHECK(token.type == type);
  BOOST_CHECK(token.value == value);
  BOOST_CHECK(token.operation == operation);
  BOOST_CHECK(token.priority == priority);
}

BOOST_AUTO_TEST_CASE(string_to_queue)
{
  std::string s = "( 1 + 2 ) * ( 3 - 4 )";
  Queue< Token > queue = stringToQueue(s);

  BOOST_TEST(queue.size() == 11);

  checkToken(queue.pop(), TokenType::left_parenthe, 0, 0, 0);
  checkToken(queue.pop(), TokenType::num, 1, 0, 0);
  checkToken(queue.pop(), TokenType::operation, 0, '+', 1);
  checkToken(queue.pop(), TokenType::num, 2, 0, 0);
  checkToken(queue.pop(), TokenType::right_parenthe, 0, 0, 0);
  checkToken(queue.pop(), TokenType::operation, 0, '*', 2);
  checkToken(queue.pop(), TokenType::left_parenthe, 0, 0, 0);
  checkToken(queue.pop(), TokenType::num, 3, 0, 0);
  checkToken(queue.pop(), TokenType::operation, 0, '-', 1);
  checkToken(queue.pop(), TokenType::num, 4, 0, 0);
  checkToken(queue.pop(), TokenType::right_parenthe, 0, 0, 0);

  BOOST_TEST(queue.empty());
}

BOOST_AUTO_TEST_CASE(calculate_valid_expression)
{
  std::string s1 = "( 1 + 2 ) * ( 3 - 4 )";
  std::string s2 = "( 5 % 3 )";
  std::string s3 = "( 5 + ( 6 * 2 ) ) - 3";
  std::string s4 = "( 8 / ( 2 + 2 ) ) + 7";
  std::string s5 = "( 9 % 5 ) * ( 3 + 1 )";
  std::string s6 = "( 4 + 5 ) * ( ( 6 - 2 ) + 3 )";
  std::string s7 = "1";
  std::string s8 = "1 + 3";
  std::string s9 = "( 10 / ( 2 + 3 ) % 4 )";
  std::string s10 = "4 * 7 - 3";
  std::string s11 = "4 >> 2";
  std::string s12 = "5 + 7 >> 1";
  std::string s13 = "( 0 - 375499 ) % 629";
  BOOST_TEST(calculate(s1) == -3);
  BOOST_TEST(calculate(s2) == 2);
  BOOST_TEST(calculate(s3) == 14);
  BOOST_TEST(calculate(s4) == 9);
  BOOST_TEST(calculate(s5) == 16);
  BOOST_TEST(calculate(s6) == 63);
  BOOST_TEST(calculate(s7) == 1);
  BOOST_TEST(calculate(s8) == 4);
  BOOST_TEST(calculate(s9) == 2);
  BOOST_TEST(calculate(s10) == 25);
  BOOST_TEST(calculate(s11) == 1);
  BOOST_TEST(calculate(s12) == 8);
  BOOST_TEST(calculate(s13) == 14);
}

BOOST_AUTO_TEST_CASE(calculate_invalid_expression)
{
 std::string expressions[] =
  {
    "",
    "( )",
    "(",
    "( 1 + 2 ",
    ")",
    "1 + 2 )",
    "( 1 + ) 2 (",
    "+ 1",
    "1 + + 2",
    "( 1 + 2 3 )",
    "( 1 + a )",
    "( 4 & 1 )",
    "( 1 + 2 ) 3"
  };

  for (size_t i = 0; i < 13; ++i)
  {
    BOOST_CHECK_THROW(calculate(expressions[i]), std::logic_error);
  }
}

BOOST_AUTO_TEST_CASE(calsulate_overflow_expression)
{
  std::string expressions[] =
  {
    "9223372036854775807 + 1",
    "9223372036854775807 + 9223372036854775807",
    "( ( 0 - 9223372036854775807 ) - 1 ) + ( 0 - 1 )",
    "( ( 0 - 9223372036854775807 ) - 1 ) - 1",
    "9223372036854775807 - ( 0 - 1 )",
    "3037000500 * 3037000500",
    "3037000500 * ( 0 - 3037000500 )",
    "9223372036854775807 * 2",
    "( ( 0 - 9223372036854775807 ) - 1 ) * 2",
    "4611686018427387904 * 2",
    "( ( 0 - 9223372036854775807 ) - 1 ) / ( 0 - 1 )",
    "( ( 0 - 9223372036854775807 ) - 1 ) % ( 0 - 1 )",
    "8 >> -1",
    "1 >> 63",
    "5 / 0"
    "5 % 0"
  };

  for (size_t i = 0; i < 14; ++i)
  {
    BOOST_CHECK_THROW(calculate(expressions[i]), std::exception);
  }
}
