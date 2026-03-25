#ifndef MATH_HPP
#define MATH_HPP

#include <climits>
#include <cstddef>
#include <stdexcept>
using ll = long long;
namespace petrov
{
  ll plus(const ll& a, const ll& b);
  ll minus(const ll& a, const ll& b);
  ll mult(const ll& a, const ll& b);
  ll div(const ll& a, const ll& b);
  ll mod(const ll& a, const ll& b);
  size_t reverseNumber(size_t num);
}

#endif // !MATH_HPP