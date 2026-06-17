#ifndef MATH_FUNCTIONS_HPP
#define MATH_FUNCTIONS_HPP

#include <iosfwd>
#include <string>
#include "stack.hpp"
#include "queue.hpp"

namespace burukov
{
  using lli_t = long long;

  lli_t add(lli_t lhs, lli_t rhs);
  lli_t sub(lli_t lhs, lli_t rhs);
  lli_t mul(lli_t lhs, lli_t rhs);
  lli_t div(lli_t lhs, lli_t rhs);
  lli_t mod(lli_t lhs, lli_t rhs);
  lli_t bitwiseAnd(lli_t lhs, lli_t rhs);

  void getInfix(std::istream &in, Stack< Queue< std::string > > &infix);
  std::string evaluateExpression(const Queue< std::string > &infix);

  namespace detail
  {
    bool isOperation(const std::string &sym);
    int getPriority(const std::string &sym);
    void convertToPostfix(const Queue< std::string > &infix, Queue< std::string > &postfix);
    lli_t applyOperation(const std::string &sym, lli_t lhs, lli_t rhs);
    lli_t calculate(const Queue< std::string > &postfix);
  }
}

#endif
