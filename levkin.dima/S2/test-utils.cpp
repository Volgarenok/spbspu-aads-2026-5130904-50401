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
  BOOST_CHECK_EQUAL(flag, 0);
}


BOOST_AUTO_TEST_CASE(to_digit_corner_case_with_chars)
{
  std::string s = "3234a";

  bool flag = 0;
  size_t result = toDigit(s, 0, s.size(), flag);
  BOOST_CHECK_EQUAL(result, 0);
  BOOST_CHECK_EQUAL(flag, 1);
}
