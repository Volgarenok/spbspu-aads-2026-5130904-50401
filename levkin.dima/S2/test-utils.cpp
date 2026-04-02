#include "utils.hpp"
#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include <string>

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
  Stack< size_t > nums;
  nums.push(2);
  nums.push(5);

  applyOp(nums, ops);
  BOOST_CHECK_EQUAL(nums.top(), 32);
  BOOST_CHECK_THROW(applyOp(nums, ops), std::runtime_error);
}
