#include "operations.hpp"
#include <limits>
#include <stdexcept>

zhuravleva::value_t zhuravleva::add(value_t a, value_t b)
{
  if ((b > 0 && a > std::numeric_limits< value_t >::max() - b) ||
      (b < 0 && a < std::numeric_limits< value_t >::min() - b))
  {
    throw std::overflow_error("overflow while addition");
  }
  return a + b;
}

zhuravleva::value_t zhuravleva::sub(value_t a, value_t b)
{
  if ((b < 0 && a > std::numeric_limits< value_t >::max() + b) ||
      (b > 0 && a < std::numeric_limits< value_t >::min() + b))
  {
    throw std::overflow_error("overflow while subtraction");
  }
  return a - b;
}

zhuravleva::value_t zhuravleva::mul(value_t a, value_t b)
{
  if (a == 0 || b == 0)
  {
    return 0;
  }
  if ((a > 0 && b > 0 && a > std::numeric_limits< value_t >::max() / b) ||
      (a < 0 && b < 0 && a < std::numeric_limits< value_t >::max() / b) ||
      (a > 0 && b < 0 && b < std::numeric_limits< value_t >::min() / a) ||
      (a < 0 && b > 0 && a < std::numeric_limits< value_t >::min() / b))
  {
    throw std::overflow_error("overflow while multiply");
  }
  return a * b;
}

zhuravleva::value_t zhuravleva::div(value_t a, value_t b)
{
  if (b == 0)
  {
    throw std::runtime_error("division by zero");
  }
  if (a == std::numeric_limits< value_t >::min() && b == -1)
  {
    throw std::overflow_error("overflow while division");
  }
  return a / b;
}

zhuravleva::value_t zhuravleva::mod(value_t a, value_t b)
{
  if (b == 0)
  {
    throw std::runtime_error("mod by zero");
  }
  value_t result = a % b;
  if (result < 0)
  {
    result += b;
  }
  return result;
}

zhuravleva::value_t zhuravleva::bitNot(value_t a)
{
  return ~a;
}
