#include "utils.hpp"
#include <stdexcept>
#include <string>
#include <boost/test/unit_test.hpp>

using namespace levkin;

BOOST_AUTO_TEST_CASE(to_digit)
{
  std::string s = "3234";

  bool flag = 0;
  size_t result = toDigit(s, 0, s.size(), flag);
  BOOST_CHECK_EQUAL(result, 3234);
  BOOST_CHECK_EQUAL(flag, 1);
}

BOOST_AUTO_TEST_CASE(to_digit_corner_case_with_chars)
{
  std::string s = "3234a";

  bool flag = 0;
  size_t result = toDigit(s, 0, s.size(), flag);
  BOOST_CHECK_EQUAL(result, 0);
  BOOST_CHECK_EQUAL(flag, 0);
}

BOOST_AUTO_TEST_CASE(get_nxt_word)
{
  std::string s = "abc 123";

  BOOST_CHECK_EQUAL(getNextWord(s, 0), 3);
  BOOST_CHECK_EQUAL(getNextWord(s, 4), s.size());
}

BOOST_AUTO_TEST_CASE(shorten_operation)
{
  std::string s = "+-*/**what?";

  BOOST_CHECK_EQUAL(encodeOpOrThrow(s, 0, 1)(2, 40), 42);
  BOOST_CHECK_EQUAL(encodeOpOrThrow(s, 1, 2)(64, 21), 43);
  BOOST_CHECK_EQUAL(encodeOpOrThrow(s, 2, 3)(11, 4), 44);
  BOOST_CHECK_EQUAL(encodeOpOrThrow(s, 3, 4)(48, 4), 12);
  BOOST_CHECK_EQUAL(encodeOpOrThrow(s, 4, 6)(2, 5), 32);
  BOOST_CHECK_THROW(encodeOpOrThrow(s, 6, s.size()), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(applying_operations)
{
  Stack< Operation > ops;
  ops.push(&exponent);
  Stack< long long > nums;
  nums.push(2);
  nums.push(5);

  applyOp(nums, ops);
  BOOST_CHECK_EQUAL(nums.top(), 32);
  BOOST_CHECK_THROW(applyOp(nums, ops), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(parse_simple_expressions)
{
  std::stringstream ss;
  ss << "10 + 20\n";
  ss << "30 - 10\n";
  ss << "5 * 5\n";

  Stack< long long > results = parse(ss);

  BOOST_CHECK_EQUAL(results.size(), 3);
  BOOST_CHECK_EQUAL(results.drop(), 25);
  BOOST_CHECK_EQUAL(results.drop(), 20);
  BOOST_CHECK_EQUAL(results.drop(), 30);
}

BOOST_AUTO_TEST_CASE(parse_complex_priority)
{
  std::stringstream ss;
  ss << "2 + 2 * 2\n";
  ss << "10 / 2 - 1\n";
  ss << "2 ** 3 * 2\n";

  Stack< long long > results = parse(ss);

  BOOST_CHECK_EQUAL(results.drop(), 16);
  BOOST_CHECK_EQUAL(results.drop(), 4);
  BOOST_CHECK_EQUAL(results.drop(), 6);
}

BOOST_AUTO_TEST_CASE(parse_parentheses)
{
  std::stringstream ss;
  ss << "( 2 + 2 ) * 2\n";
  ss << "10 / ( 2 + 3 )\n";

  Stack< long long > results = parse(ss);

  BOOST_CHECK_EQUAL(results.drop(), 2);
  BOOST_CHECK_EQUAL(results.drop(), 8);
}

BOOST_AUTO_TEST_CASE(parse_errors)
{
  std::stringstream ss1("10 / 0\n");
  BOOST_CHECK_THROW(parse(ss1), std::runtime_error);

  std::stringstream ss3("1 ? 2\n");
  BOOST_CHECK_THROW(parse(ss3), std::runtime_error);

  std::stringstream ss4("1 + \n");
  BOOST_CHECK_THROW(parse(ss4), std::runtime_error);
}
