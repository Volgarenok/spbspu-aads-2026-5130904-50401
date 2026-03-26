#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <sstream>
#include <string>

#include "Token.hpp"

BOOST_AUTO_TEST_SUITE(token_reading_suite)

using namespace khasnulin;

BOOST_AUTO_TEST_CASE(test_read_basic_tokens)
{
  std::string ss("123 + ( ) *");
  size_t pos = 0;

  Token t1 = readToken(ss, pos);
  BOOST_CHECK_EQUAL(t1.type, TokenType::Number);
  BOOST_CHECK_EQUAL(t1.value, 123);

  Token t2 = readToken(ss, pos);
  BOOST_CHECK_EQUAL(t2.type, TokenType::Operator);
  BOOST_CHECK_EQUAL(t2.op, '+');
  BOOST_CHECK_EQUAL(t2.priority, 1);

  Token t3 = readToken(ss, pos);
  BOOST_CHECK_EQUAL(t3.type, TokenType::LeftParen);

  Token t4 = readToken(ss, pos);
  BOOST_CHECK_EQUAL(t4.type, TokenType::RightParen);

  Token t5 = readToken(ss, pos);
  BOOST_CHECK_EQUAL(t5.op, '*');
  BOOST_CHECK_EQUAL(t5.priority, 2);
}

BOOST_AUTO_TEST_CASE(test_read_end_of_stream)
{
  std::string ss("42");
  size_t pos = 0;
  readToken(ss, pos);
  Token t = readToken(ss, pos);

  BOOST_CHECK_EQUAL(t.type, TokenType::EndOfExpr);
}

BOOST_AUTO_TEST_CASE(test_number_overflow)
{
  std::string ss("99999999999999999999");
  size_t pos = 0;
  BOOST_CHECK_THROW(readToken(ss, pos), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(test_max_int)
{
  std::string ss("2147483647");
  size_t pos = 0;
  Token t = readToken(ss, pos);
  BOOST_CHECK_EQUAL(t.value, 2147483647);
}

BOOST_AUTO_TEST_CASE(test_invalid_tokens)
{
  std::string ss("123a @ ?");
  size_t pos = 0;
  BOOST_CHECK_THROW(readToken(ss, pos), std::runtime_error);

  BOOST_CHECK_THROW(readToken(ss, pos), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_multichar_operators)
{
  std::string ss("++ == **");
  size_t pos = 0;
  BOOST_CHECK_THROW(readToken(ss, pos), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_whitespace_handling)
{
  std::string ss("   42      +     ");
  size_t pos = 0;
  Token t1 = readToken(ss, pos);
  BOOST_CHECK_EQUAL(t1.value, 42);
  Token t2 = readToken(ss, pos);
  BOOST_CHECK_EQUAL(t2.op, '+');
}

BOOST_AUTO_TEST_CASE(test_multiline_input)
{
  std::stringstream ss("1 + 2\n3 * 4");

  std::string s;

  std::getline(ss, s);
  size_t pos = 0;

  BOOST_CHECK_EQUAL(readToken(s, pos).value, 1);
  BOOST_CHECK_EQUAL(readToken(s, pos).op, '+');
  BOOST_CHECK_EQUAL(readToken(s, pos).value, 2);
  BOOST_CHECK_EQUAL(readToken(s, pos).type, TokenType::EndOfExpr);

  std::getline(ss, s);
  pos = 0;
  BOOST_CHECK_EQUAL(readToken(s, pos).value, 3);
  BOOST_CHECK_EQUAL(readToken(s, pos).op, '*');
  BOOST_CHECK_EQUAL(readToken(s, pos).value, 4);
  BOOST_CHECK_EQUAL(readToken(s, pos).type, TokenType::EndOfExpr);
}

BOOST_AUTO_TEST_CASE(test_empty_line)
{
  std::stringstream ss("\n\n");
  std::string s;
  std::getline(ss, s);
  size_t pos = 0;
  BOOST_CHECK_EQUAL(readToken(s, pos).type, TokenType::EndOfExpr);
  std::getline(ss, s);
  pos = 0;
  BOOST_CHECK_EQUAL(readToken(s, pos).type, TokenType::EndOfExpr);
  std::getline(ss, s);
  pos = 0;
  BOOST_CHECK_EQUAL(readToken(s, pos).type, TokenType::EndOfExpr);
}

BOOST_AUTO_TEST_SUITE_END()
