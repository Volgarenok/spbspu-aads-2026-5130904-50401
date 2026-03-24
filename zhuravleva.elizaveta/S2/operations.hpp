#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP

#include <stdexcept>
#include <limits>

namespace zhuravleva
{
  using value_t = long long;
  value_t add(value_t a, value_t b)
  {
    if ((b > 0 && a > std::numeric_limits<value_t>::max() - b) ||
        (b < 0 && a < std::numeric_limits<value_t>::min() - b))
    {
      throw std::overflow_error("overflow while addition");
    }
    return a + b;
  }

  value_t sub(value_t a, value_t b)
  {
    if ((b < 0 && a > std::numeric_limits<value_t>::max() + b) ||
        (b > 0 && a < std::numeric_limits<value_t>::min() + b))
    {
      throw std::overflow_error("overflow while subtraction");
    }
    return a - b;
  }

  value_t mul(value_t a, value_t b)
  {
    if (a == 0 || b == 0)
    {
      return 0;
    }
    if (a > std::numeric_limits<value_t>::max() / b ||
        a < std::numeric_limits<value_t>::min() / b)
    {
      throw std::overflow_error("overflow while multiply");
    }
    return a * b;
  }

  value_t div(value_t a, value_t b)
  {
    if (b == 0)
    {
      throw std::runtime_error("division by zero");
    }
    return a / b;
  }

  value_t mod(value_t a, value_t b)
  {
    if (b == 0)
    {
      throw std::runtime_error("mod by zero");
    }
    return a % b;
  }

  value_t bitNot(value_t a)
  {
    return ~a;
  }
}

#endif