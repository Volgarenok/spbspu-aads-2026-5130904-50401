#ifndef MATH_EXPRESSION_HPP
#define MATH_EXPRESSION_HPP

#include <string>
#include "queue.hpp"

namespace studilova
{
  bool isNumber(const std::string& s);
  bool isOperator(const std::string& s);
  int getPrecedence(const std::string& op);

  studilova::Queue< std::string >infixToPostfix(const std::string& line);
  long long evaluatePostfix(studilova::Queue< std::string >& postfix);

  long long evaluateExpression(const std::string& line);
}

#endif
