#ifndef EXPRESSION_PROCESS_HPP
#define EXPRESSION_PROCESS_HPP

#include "Queue.hpp"
#include "Stack.hpp"
#include "Token.hpp"

namespace khasnulin
{
  Queue< Token > readExpression(std::istream &in);

  Stack< Queue< Token > > readAllExpressions(std::istream &in);
}

#endif
