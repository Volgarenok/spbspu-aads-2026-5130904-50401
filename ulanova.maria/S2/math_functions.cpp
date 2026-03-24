#include "math_functions.hpp"

namespace ulanova
{
  bool is_operator(char c)
  {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
  }
  int priority(char op)
  {
    if (op == '+' || op == '-' || op == '^')
    {
      return 1;
    }
    if ( op == '*' || op == '/' || op == '%')
    {
      return 2;
    }
    return 0;
  }
}
