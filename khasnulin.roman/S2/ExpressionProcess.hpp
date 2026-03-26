#ifndef EXPRESSION_PROCESS_HPP
#define EXPRESSION_PROCESS_HPP

#include "Queue.hpp"
#include "Stack.hpp"
#include "Token.hpp"

namespace khasnulin
{
  Queue< Token > readExpression(std::istream &in);

  int readAndProcessExpressionLine(const std::string &line);

  void processToken(Token &token, Stack< Token > &values, Stack< Token > &operations);

  Stack< Queue< Token > > readAllExpressions(std::istream &in);
}

#endif
