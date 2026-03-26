#ifndef MATH_FUNCS_HPP
#define MATH_FUNCS_HPP

#include <limits>
#include <string>

namespace chernov {
  constexpr long long MIN_LLI = std::numeric_limits< long long >::min();
  constexpr long long MAX_LLI = std::numeric_limits< long long >::max();

  long long abs(long long a);
  long long add(long long a, long long b);
  long long sub(long long a, long long b);
  long long mod(long long a, long long b);
  long long div(long long a, long long b);
  long long mul(long long a, long long b);
  long long gcd(long long a, long long b);
  long long lcm(long long a, long long b);

  bool isOperator(const std::string & str);
}

#endif
