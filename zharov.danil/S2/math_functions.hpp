#ifndef MATH_FUNCTIONS_HPP
#define MATH_FUNCTIONS_HPP
#include <limits>

namespace zharov {
  using ll_t = long long int;
  constexpr ll_t MAX = std::numeric_limits< ll_t >::max();
  constexpr ll_t MIN = std::numeric_limits< ll_t >::min();
  ll_t add(const ll_t & a, const ll_t & b);
  ll_t sub(const ll_t & a, const ll_t & b);
  ll_t mul(const ll_t & a, const ll_t & b);
  ll_t div(const ll_t & a, const ll_t & b);
  ll_t mod(const ll_t & a, const ll_t & b);
  ll_t bitShiftLeft(const ll_t & a, const ll_t & b);
}

#endif
