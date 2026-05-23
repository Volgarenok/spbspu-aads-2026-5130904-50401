#ifndef MATH_OP_HPP
#define MATH_OP_HPP
#include <string>
#include "queue.hpp"
#include "stack.hpp"

namespace vasyakin
{
  bool isOperation(const std::string& s);
  int getPrecedence(const std::string& op);
  long long gcd(long long a, long long b);
  long long lcm(long long a, long long b);
  long long sum(long long a, long long b);
  long long subtract(long long a, long long b);
  long long multiply(long long a, long long b);
  long long divide(long long a, long long b);
  long long mod(long long a, long long b);
  long long calculate(long long a, long long b, const std::string& op);
  void processToken(const std::string& token, Stack< std::string >& opStack,
    Queue< std::string >& postfix);
  Queue< std::string > infixToPostfix(const std::string& line);
  long long evaluatePostfix(Queue< std::string >& postfix);
  long long evaluateExpression(const std::string& line);
  const long long min = std::numeric_limits< long long >::min();
  const long long maxLL = std::numeric_limits< long long >::max();
}

#endif
