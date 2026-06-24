#include "math_functions.hpp"
#include <climits>

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
      case '+' :
        if ((b > 0 && a > LLONG_MAX - b ) || (b < 0 && LLONG_MIN - b))
        {
          throw std::overflow_error("overflow");
        }
        return a + b;
      case '-' :
        if (( b < 0 && a > LLONG_MAX + b) || (b > 0 && a < LLONG_MIN + b))
        {
          throw std::overflow_error("underflow");
        }
        return a - b;
      case '*' :
        if (a != 0 && (b > LLONG_MAX / a || b < LLONG_MIN / a))
        {
          throw std::overflow_error("overflow");
        }
        return a * b;
      case '/' :
        if ( b == 0)
        {
          throw std::runtime_error("division by zero");
        }
        if (a == LLONG_MIN && b == -1)
        {
          throw std::overflow_error("overflow");
        }
        return a / b;
      case '%' :
        if ( b == 0)
        {
          throw std::runtime_error("modul by zero");
        }
        return ((a % b) + b) % b;
      case '^' : return a ^ b;
      default:
        throw std::runtime_error("unknown operator");
    }
  }
}
