#ifndef MATH_FUNCTIONS_HPP
#define MATH_FUNCTIONS_HPP

#include <iostream>
#include <string>
#include "queue.hpp"
#include "stack.hpp"

namespace burukov
{
  using lli_t = long long;
  using func_t = lli_t(*)(lli_t, lli_t);

  bool isOperand(const std::string &sym);
  int getPriority(const std::string &sym);

  lli_t add(lli_t lhs, lli_t rhs);
  lli_t sub(lli_t lhs, lli_t rhs);
  lli_t mul(lli_t lhs, lli_t rhs);
  lli_t div(lli_t lhs, lli_t rhs);
  lli_t mod(lli_t lhs, lli_t rhs);
  lli_t bitwiseAnd(lli_t lhs, lli_t rhs);

  void getInfix(std::istream &in,
      Stack< Queue< std::string > > &infix);
  void convertToPostfix(const Queue< std::string > &infix,
      Queue< std::string > &postfix);
  std::string calculate(const Queue< std::string > &postfix);
}

#endif
