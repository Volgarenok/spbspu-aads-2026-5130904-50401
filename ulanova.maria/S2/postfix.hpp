#ifndef POSTFIX_HPP
#define POSTFIX_HPP

#include "stack.hpp"
#include "math_functions.hpp"
#include <string>
#include <sstream>

namespace ulanova
{
  long long postfix(const std::string& expr)
  {
    Stack<long long> stack;
    std::stringstream s(expr);
    std::string token;
    while (s >> token)
    {
      if (token.size() == 1 && is_operator(token[0]))
      {
        long long b = stack.drop();
        long long a = stack.drop();
        long long res = apply_op(a, b, token[0]);
        stack.push(res);
      }
      else
      {
        long long value = std::stoll(token);
        stack.push(value);
      }
    }
    return stack.drop();
  }
}
#endif
