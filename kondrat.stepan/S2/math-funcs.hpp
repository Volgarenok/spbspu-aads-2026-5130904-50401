#ifndef MATH_FUNCS_HPP
#define MATH_FUNCS_HPP
#include <cstddef>
#include <string>
#include <istream>
#include <ostream>
#include <limits>
#include <stdexcept>
#include "queue.hpp"
#include "stack.hpp"

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
  ll evaluatePostfix(Queue< std::string > & postfix);

  size_t getOperatorType(const std::string & token);
  bool isOperator(const std::string & token);
  size_t getPriority(const std::string & token);

  Queue< std::string > tokenize(const std::string & str);
  Queue< std::string > infixToPostfix(Queue< std::string > & before);

  void readExpressions(std::istream & in, Stack< ll > & result);
  void printResults(Stack< ll > & result, std::ostream & in);
}

#endif