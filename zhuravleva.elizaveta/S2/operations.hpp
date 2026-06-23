#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP

namespace zhuravleva
{
  using value_t = long long;

  value_t add(value_t a, value_t b);
  value_t sub(value_t a, value_t b);
  value_t mul(value_t a, value_t b);
  value_t div(value_t a, value_t b);
  value_t mod(value_t a, value_t b);
  value_t bitNot(value_t a);
}

#endif
