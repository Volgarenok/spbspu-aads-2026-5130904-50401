#ifndef MATH_EXPRESSION_HPP
#define MATH_EXPRESSION_HPP

#include <string>
#include <limits>
#include "queue.hpp"
#include "stack.hpp"

namespace studilova
{
  bool isNumber(const std::string& s);
  bool isOperator(const std::string& s);
  int getPrecedence(const std::string& op);
  bool isRightAssociative(const std::string& op);
  long long applyOperator( long long a, long long b, const std::string& op);
  void processToken(const std::string& token, studilova::Stack< std::string >& ops, studilova::Queue< std::string >& output);

  long long add(long long a, long long b);
  long long subtract(long long a, long long b);
  long long multiply(long long a, long long b);
  long long divide(long long a, long long b);
  long long mod(long long a, long long b);
  long long power(long long a, long long b);

  studilova::Queue< std::string >infixToPostfix(const std::string& line);
  long long evaluatePostfix(studilova::Queue< std::string >& postfix);

  long long evaluateExpression(const std::string& line);

  const long long MAX = std::numeric_limits< long long >::max();
  const long long MIN = std::numeric_limits< long long >::min();
}

#endif
