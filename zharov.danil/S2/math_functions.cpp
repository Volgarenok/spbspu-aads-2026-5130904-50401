#include <climits>
#include <stdexcept>
#include "math_functions.hpp"

using ll_t = zharov::ll_t;

ll_t zharov::add(const ll_t & a, const ll_t & b)
{
  if ((b > 0 && a > MAX - b) || (b < 0 && a < MIN - b)){
    throw std::overflow_error("Overflow in add");
  }
  return a + b;
}

ll_t zharov::sub(const ll_t & a, const ll_t & b)
{
  if ((b > 0 && a < MIN + b ) || (b < 0 && a > MAX + b)) {
    throw std::overflow_error("Overflow in sub");
  }
  return a - b;
}

ll_t zharov::mul(const ll_t & a, const ll_t & b)
{
  if (a == 0 || b == 0) {
    return 0;
  }
  if ((a == -1 && b == MIN) || (b == -1 && a == MIN)) {
    throw std::overflow_error("Overflow in mul");
  }
  if ((a > 0 && b > 0 && a > MAX / b) ||
    (a > 0 && b < 0 && b < MIN / a) ||
    (a < 0 && b > 0 && a < MIN / b) ||
    (a < 0 && b < 0 && a < MAX / b)) {
      throw std::overflow_error("Overflow in mul");
    }
  return a * b;
}

ll_t zharov::div(const ll_t & a, const ll_t & b)
{
  ll_t MIN = std::numeric_limits< ll_t >::min();
  if (b == 0) {
    throw std::logic_error("Div by zero");
  }
  if (a == MIN && b == -1) {
    throw std::overflow_error("Overflow in div");
  }
  return a / b;
}

ll_t zharov::mod(const ll_t & a, const ll_t & b)
{
  if (b == 0) {
    throw std::logic_error("Mod by zero");
  }
  ll_t result = a % b;
  if (result < 0) {
    result += (b > 0) ? b : -b;
  }
  return result;
}

ll_t zharov::bitShiftLeft(const ll_t & a, const ll_t & b)
{
  ll_t width = sizeof(ll_t) * CHAR_BIT;
  if (a < 0) {
    throw std::overflow_error("Cannot shift negative number");
  }
  if (b < 0 || b >= width) {
    throw std::overflow_error("Invalid shift amount");
  }
  if (a > (MAX >> b)) {
    throw std::overflow_error("Overflow in bitShiftLeft");
  }
  return a << b;
}
