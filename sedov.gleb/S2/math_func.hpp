#ifndef MATH_FUNC_HPP
#define MATH_FUNC_HPP
#include <iostream>
#include <string>
#include <limits>
#include "queue.hpp"
#include "stack.hpp"
#include "expression.hpp"

namespace sedov
{
  using lli_t = long long int;
  using func_t = lli_t(*)(const lli_t &, const lli_t &);

  lli_t add(const lli_t & a, const lli_t & b);
  lli_t sub(const lli_t & a, const lli_t & b);
  lli_t mult(const lli_t & a, const lli_t & b);
  lli_t div(const lli_t & a, const lli_t & b);
  lli_t mod(const lli_t & a, const lli_t & b);
  lli_t bitRightShift(const lli_t & a, const lli_t & b);

  void getInfix(std::istream & in, Stack< Expression > & infix);
  Expression convertInfToPost(const Expression & infix);
  std::string calculate(const Expression & postfix);
}

#endif
