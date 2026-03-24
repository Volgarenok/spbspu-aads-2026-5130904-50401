#include <limits>
#include <climits>
#include <stdexcept>
#include "math_functions.hpp"

using ll_t = zharov::ll_t;




ll_t add(const ll_t & a, const ll_t & b)
{
  ll_t max = std::numeric_limits< ll_t >::max();
  ll_t min = std::numeric_limits< ll_t >::min();
  if ((b > 0 && a > max - b) || (b < 0 && a < min - b)){
    throw std::overflow_error("Overflow in add");
  }
  return a + b;
}

ll_t sub(const ll_t & a, const ll_t & b)
{
  ll_t max = std::numeric_limits< ll_t >::max();
  ll_t min = std::numeric_limits< ll_t >::min();
  if ((b > 0 && a < min + b ) || (b < 0 && a > max + b)) {
    throw std::overflow_error("Overflow in sub");
  }
  return a - b;
}

ll_t mul(const ll_t & a, const ll_t & b)
{
  if (a == 0 || b == 0) {
    return 0;
  }
  ll_t max = std::numeric_limits< ll_t >::max();
  ll_t min = std::numeric_limits< ll_t >::min();
  if ((a == -1 && b == min) || (b == -1 && a == min)) {
    throw std::overflow_error("Overflow in mul");
  }
  if ((a > 0 && b > 0 && a > max / b) ||
    (a > 0 && b < 0 && b < min / a) || 
    (a < 0 && b > 0 && a < min / b) ||
    (a < 0 && b < 0 && a < max / b)) {
      throw std::overflow_error("Overflow in mul");
    }
  return a * b;
}

ll_t div(const ll_t & a, const ll_t & b)
{
  ll_t max = std::numeric_limits<ll_t>::max();
  ll_t min = std::numeric_limits<ll_t>::min();
  if (b == 0) {
    throw std::logic_error("Div by zero");
  }
  if (a == min && b == -1) {
    throw std::overflow_error("Overflow in div");
  }
  return a / b;
}

ll_t mod(const ll_t & a, const ll_t & b)
{
  ll_t min = std::numeric_limits<ll_t>::min();

  if (b == 0) {
    throw std::logic_error("Mod by zero");
  }
  if (a == min && b == -1) {
    throw std::overflow_error("Overflow in mod");
  }
  return a % b;
}

ll_t bitShiftLeft(const ll_t & a, const ll_t & b)
{
  ll_t max = std::numeric_limits<ll_t>::max();
  ll_t width = sizeof(ll_t) * CHAR_BIT;
  if (a < 0) {
    throw std::overflow_error("Cannot shift negative number");
  }
  if (b < 0 || b >= width) {
    throw std::overflow_error("Invalid shift amount");
  }
  if (a > (max >> b)) {
    throw std::overflow_error("Overflow in bitShiftLeft");
  }
  return a << b;
}
