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
  long long apply_op(long long a, long long b, char op)
  {
    switch (op)
    {
      case '+' : return a + b;
      case '-' : return a - b;
      case '*' : return a * b;
      case '/' :
        if ( b == 0)
        {
          throw std::runtime_error("division by zero");
        }
        return a / b;
      case '%' :
        if ( b == 0)
        {
          throw std::runtime_error("modul by zero");
        }
        return a % b;
      case '^' : return a ^ b;
      default:
        throw std::runtime_error("unknown operator");
    }
  }
}
