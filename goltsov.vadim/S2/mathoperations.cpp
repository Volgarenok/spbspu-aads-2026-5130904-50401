#include <limits>
#include "mathoperations.hpp"
#include <iostream>

namespace goltsov
{
  using lli = long long int;
  lli MAX_LLI = std::numeric_limits< lli >::max();
  lli MIN_LLI = std::numeric_limits< lli >::min();
  lli addition(lli a, lli b)
  {
    if (a > 0 && b > 0 && a > MAX_LLI - b || a < 0 && b < 0 && a < MIN_LLI - b)
    {
      throw std::overflow_error("Overflow addition");
    }
    return a + b;
  }
  lli subtraction(lli a, lli b)
  {
    if (a < 0 && b > 0 && a < MIN_LLI + b || a > 0 && b < 0 && a > MAX_LLI + b)
    {
      throw std::overflow_error("Overflow subtraction");
    }
    return a - b;
  }
  lli multiplication(lli a, lli b)
  {
    if (b > 0 && a > MAX_LLI / b)
    {
      throw std::overflow_error("Overflow multiplication");
    }
    if (b < 0 && a < MAX_LLI / b)
    {
      throw std::overflow_error("Overflow multiplication");
    }
    if (a > 0 && b < 0 && b < MIN_LLI / a)
    {
      throw std::overflow_error("Overflow multiplication");
    }
    if (a < 0 && b > 0 && a < MIN_LLI / b)
    {
      throw std::overflow_error("Overflow multiplication");
    }  
    return a * b;
  }
  lli division(lli a, lli b)
  {
    return a / b;
  }
  lli remOfDiv(lli a, lli b)
  {
    return a % b;
  }
  lli concatenation(lli a, lli b)
  {
    lli res;
    try
    {
      res = addition(multiplication(a, pow(10, countDigitOfNumber(b))), b);
    }
    catch(...)
    {
      throw std::overflow_error("Overflow addition");
    }
    return res;
  }
  lli countDigitOfNumber(lli a)
  {
    lli ans = 0;
    while (a != 0)
    {
      ans++;
      a /= 10;
    }
    return ans;
  }
  lli pow(lli a, lli b)
  {
    lli res = 1;
    while (b != 0)
    {
      res = multiplication(res, a);
      --b;
    }
    return res;
  }
}
