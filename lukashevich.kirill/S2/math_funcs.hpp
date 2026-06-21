#ifndef MATH_FUNCS_HPP
#define MATH_FUNCS_HPP

#include <string>
#include <cstddef>

#include <list/list.hpp>

#include "stack.hpp"
#include "queue.hpp"

namespace lukashevich
{
  using ll = long long;

  namespace detail
  {
    size_t getOperatorType(const std::string& token);
    bool isOperator(const std::string& token);
    size_t getPriority(const std::string& token);
  }

  List< std::string > tokenize(const std::string& str);
  Queue< std::string > infixToPostfix(const List< std::string >& tokens);

  ll add(ll lhs, ll rhs);
  ll sub(ll lhs, ll rhs);
  ll mul(ll lhs, ll rhs);
  ll divide(ll lhs, ll rhs);
  ll mod(ll lhs, ll rhs);
  ll bitwise_not(ll value);
  ll parse_ll(const std::string& s);
  ll evaluate_expression(const std::string& line);
}
#endif
