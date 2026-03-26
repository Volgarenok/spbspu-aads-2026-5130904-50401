#ifndef MATH_FUNCS_HPP
#define MATH_FUNCS_HPP

#include <limits>
#include <string>

#include <stack.hpp>
#include <queue.hpp>

namespace chernov {
  constexpr long long MIN_LLI = std::numeric_limits< long long >::min();
  constexpr long long MAX_LLI = std::numeric_limits< long long >::max();

  constexpr size_t count_operators = 8;
  const std::string operators[count_operators] = {"+", "-", "*", "/", "%", "lcm", "(", ")"};

  long long abs(long long a);
  long long add(long long a, long long b);
  long long sub(long long a, long long b);
  long long mod(long long a, long long b);
  long long div(long long a, long long b);
  long long mul(long long a, long long b);
  long long gcd(long long a, long long b);
  long long lcm(long long a, long long b);

  bool isOperator(const std::string & str);
  bool isOperand(const std::string & str);
  size_t getPriority(const std::string & oper);

  void executeOperation(Stack< long long > & result, const std::string & oper);
  long long calculateMathExpression(Queue< std::string > math_expression);
}

#endif
