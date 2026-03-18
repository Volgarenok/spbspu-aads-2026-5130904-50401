#ifndef MATH_OP_HPP
#define MATH_OP_HPP
#include <string>
#include "queue.hpp"

namespace vasyakin
{
  bool isNumber(const std::string& s);
  bool isOperator(const std::string& s);
  int getPrecedence(const std::string& op);
  long long gcd(long long a, long long b);
  long long lcm(long long a, long long b);
  long long sum(long long a, long long b);
  long long subtract(long long a, long long b);
  long long multiply(long long a, long long b);
  long long divide(long long a, long long b);
  long long mod(long long a, long long b);
  long long calculate(long long a, long long b, const std::string& op);
  Queue< std::string > infixToPostfix(const std::string& line);
  long long evaluatePostfix(Queue< std::string >& postfix);
  long long evaluateExpression(const std::string& line);
  const long long MIN = std::numeric_limits< long long >::min();
}

#endif
