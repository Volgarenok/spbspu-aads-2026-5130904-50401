#ifndef MATH_FUNCS_HPP
#define MATH_FUNCS_HPP
#include <cstddef>
#include <string>
#include "../common/includes/stack.hpp"
#include "../common/includes/queue.hpp"

namespace kuznetsov {
  bool isOperand(const std::string& c);
  size_t getPriority(const std::string& c);
  using lli_t = long long;

  lli_t add(const lli_t& a, const lli_t& b);
  lli_t sub(const lli_t& a, const lli_t& b);
  lli_t mul(const lli_t& a, const lli_t& b);
  lli_t div(const lli_t& a, const lli_t& b);
  lli_t mod(const lli_t& a, const lli_t& b);
  lli_t bitShiftToRight(const lli_t& a, const lli_t& b);

  using stackOfinfixExpression = Stack< Queue< std::string > >;
  void getExpressions(std::istream& in,  stackOfinfixExpression res);
  void calculate(stackOfinfixExpression infix, Queue< lli_t > res);
}

#endif

