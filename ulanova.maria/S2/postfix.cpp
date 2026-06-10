#include "postfix.hpp"
#include "math_functions.hpp"
#include "stack.hpp"

#include <sstream>
#include <string>
#include <stdexcept>

namespace
{
  bool isOperator(char c)
  {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
  }
}

long long ulanova::postfix(Queue< std::string >& expr)
{
  Stack< long long > stack;

  while (!expr.empty())
  {
    std::string token = expr.front();
    expr.pop();

    if (token.size() == 1 && isOperator(token[0]))
    {
      long long b = stack.front();
      stack.pop();

      long long a = stack.front();
      stack.pop();

      stack.push(apply_op(a, b, token[0]));
    }
    else
    {
      stack.push(std::stoll(token));
    }
  }
  if (stack.empty())
  {
    throw std::runtime_error("incorrect expression");
  }
  long long result = stack.front();
  stack.pop();
  if (!stack.empty())
  {
    throw std::runtime_error("incorrect expression");
  }

  return result;
}
