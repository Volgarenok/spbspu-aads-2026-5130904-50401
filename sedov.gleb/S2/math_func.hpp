#ifndef MATH_FUNC_HPP
#define MATH_FUNC_HPP
#include <iostream>
#include "queue.hpp"
#include "stack.hpp"
#include <string>
#include <limits>

namespace sedov
{
  using lli_t = long long int;

  bool isOperand(const std::string & sym);
  size_t getPriority(const std::string & sym);
  lli_t add(const lli_t & a, const lli_t & b);
  lli_t sub(const lli_t & a, const lli_t & b);
  lli_t mult(const lli_t & a, const lli_t & b);
  lli_t div(const lli_t & a, const lli_t & b);
  lli_t mod(const lli_t & a, const lli_t & b);
  lli_t bitRightShift(const lli_t & a, const lli_t & b);
}
#endif
