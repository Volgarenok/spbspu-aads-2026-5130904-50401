#ifndef EXPRESSION_PROCESS_HPP
#define EXPRESSION_PROCESS_HPP

#include "Stack.hpp"
#include "Token.hpp"

namespace khasnulin
{
  Stack< int > readAndProcessAllExpressions(std::istream &in);

  int readAndProcessExpressionLine(const std::string &line);

  void processToken(Token &token, Stack< Token > &values, Stack< Token > &operations);
}

#endif
