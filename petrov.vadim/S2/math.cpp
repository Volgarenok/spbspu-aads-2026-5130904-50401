#include "math.hpp"

namespace petrov
{
  ll plus(const ll& a, const ll& b)
  {
    if (b > 0 && a > LLONG_MAX - b)
    {
      throw std::overflow_error("Overflow in addition");
    }
    if (b < 0 && a < LLONG_MIN - b)
    {
      throw std::overflow_error("Underflow in addition");
    }
    return a + b;
  }

  ll minus(const ll& a, const ll& b)
  {
    if (b < 0 && a > LLONG_MAX + b)
    {
      throw std::overflow_error("Overflow in subtraction");
    }
    if (b > 0 && a < LLONG_MIN + b)
    {
      throw std::overflow_error("Underflow in subtraction");
    }
    return a - b;
  }

  ll mult(const ll& a, const ll& b)
  {
    if (a == 0 || b == 0)
    {
      return 0;
    }
    if (a == -1 && b == LLONG_MIN)
    {
      throw std::overflow_error("Overflow in multiplication");
    }
    if (b == -1 && a == LLONG_MIN)
    {
      throw std::overflow_error("Overflow in multiplication");
    }
    ll result = a * b;
    if (a == result / b)
    {
      return result;
    }
    throw std::overflow_error("Overflow in multiplication");
  }

  ll div(const ll& a, const ll& b)
  {
    if (b == 0)
    {
      throw std::invalid_argument("Division by zero");
    }
    if ((a == LLONG_MIN && b == -1))
    {
      throw std::overflow_error("Overflow in division");
    }
    return a / b;
  }

  ll mod(const ll& a, const ll& b)
  {
    if (b == 0)
    {
      throw std::invalid_argument("Division by zero");
    }
    if ((a == LLONG_MIN && b == -1))
    {
      throw std::overflow_error("Overflow in modulus");
    }
    return a % b;
  }

  ll reverseNumber(ll num)
  {
    ll res = 0;
    while (num != 0)
    {
      int digit = num % 10;
      if (res > LLONG_MAX / 10 || res < LLONG_MIN / 10)
      {
          throw std::overflow_error("Overflow in reverse");
      }
      res = res * 10 + digit;
      num /= 10;
    }
    return res;
  }
}