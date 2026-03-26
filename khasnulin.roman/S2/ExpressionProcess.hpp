#ifndef EXPRESSION_PROCESS_HPP
#define EXPRESSION_PROCESS_HPP

#include "Stack.hpp"
#include "Token.hpp"

namespace khasnulin
{
  Stack< long long > readAndProcessAllExpressions(std::istream &in);

  long long readAndProcessExpressionLine(const std::string &line);

  void processToken(Token &token, Stack< Token > &values, Stack< Token > &operations);
}

#endif
