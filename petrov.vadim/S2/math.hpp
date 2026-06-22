#ifndef MATH_HPP
#define MATH_HPP

#include <climits>
#include <cstddef>
#include <stdexcept>
namespace petrov
{
  using ll = long long;

  ll plus(const ll& a, const ll& b);
  ll minus(const ll& a, const ll& b);
  ll mult(const ll& a, const ll& b);
  ll div(const ll& a, const ll& b);
  ll mod(const ll& a, const ll& b);
  ll reverseNumber(const ll& num);
}

#endif
