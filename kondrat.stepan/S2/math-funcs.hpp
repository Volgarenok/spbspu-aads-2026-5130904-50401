#ifndef MATH_FUNCS_HPP
#define MATH_FUNCS_HPP

#include <string>

namespace kondrat
{
  using ll = long long;

  ll add(ll lhs, ll rhs);
  ll sub(ll lhs, ll rhs);
  ll mul(ll lhs, ll rhs);
  ll divide(ll lhs, ll rhs);
  ll mod(ll lhs, ll rhs);
  ll reverseNumber(ll value);
  ll applyBinaryOperation(ll lhs, ll rhs, const std::string & token);
  ll applyUnaryOperation(ll value, const std::string & token);
  ll calculateExpression(const std::string & str);
}

#endif
