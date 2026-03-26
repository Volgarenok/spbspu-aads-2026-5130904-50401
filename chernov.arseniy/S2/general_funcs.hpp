#ifndef GENERAL_FUNCS_HPP
#define GENERAL_FUNCS_HPP

#include <string>

#include <stack.hpp>
#include <queue.hpp>

namespace chernov {
  Queue< std::string > processLine(const std::string & line);
  void executeOperation(Stack< long long > & result, const std::string & oper);
  long long calculateMathExpression(Queue< std::string > math_expression);
}

#endif
