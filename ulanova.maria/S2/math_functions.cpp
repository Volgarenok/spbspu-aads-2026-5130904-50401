#include "math_functions.hpp"
#include <climits>
#include <stdexcept>

namespace
{
  long long add(long long a, long long b)
  {
    if ((b > 0 && a > LLONG_MAX - b) || (b < 0 && a < LLONG_MIN - b))
    {
      throw std::overflow_error("overflow");
    }
    return a + b;
  }

  long long subtract(long long a, long long b)
  {
    if ((b < 0 && a > LLONG_MAX + b) || (b > 0 && a < LLONG_MIN + b))
    {
      throw std::overflow_error("underflow");
    }
    return a - b;
  }

  long long multiply(long long a, long long b)
  {
    if (a > 0)
    {
      if (b > 0 && a > LLONG_MAX / b)
      {
        throw std::overflow_error("overflow");
      }
      if (b < 0 && b < LLONG_MIN / a)
      {
        throw std::overflow_error("underflow");
      }
    }
    else if (a < 0)
    {
      if (b > 0 && a < LLONG_MIN / b)
      {
        throw std::overflow_error("underflow");
      }
      if (b < 0 && a < LLONG_MAX / b)
      {
        throw std::overflow_error("overflow");
      }
    }
    return a * b;
  }

  long long divide(long long a, long long b)
  {
    if (b == 0)
    {
      throw std::runtime_error("division by zero");
    }
    if (a == LLONG_MIN && b == -1)
    {
      throw std::overflow_error("overflow");
    }
    return a / b;
  }

  long long mod(long long a, long long b)
  {
    if (b == 0)
    {
      throw std::runtime_error("modul by zero");
    }
    return ((a % b) + b) % b;
  }

  long long Xor(long long a, long long b)
  {
    return a ^ b;
  }
}

long long ulanova::apply_op(long long a, long long b, char op)
{
  switch (op)
  {
    case '+':
      return add(a, b);
    case '-':
      return subtract(a, b);
    case '*':
      return multiply(a, b);
    case '/':
      return divide(a, b);
    case '%':
      return mod(a, b);
    case '^':
      return Xor(a, b);
    default:
      throw std::runtime_error("unknown operator");
  }
}
