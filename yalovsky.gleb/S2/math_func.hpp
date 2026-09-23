#ifndef YALOVSKY_MATH_FUNC_HPP
#define YALOVSKY_MATH_FUNC_HPP

#include <iosfwd>
#include <string>

#include "queue.hpp"
#include "stack.hpp"

namespace yalovsky
{
  using lli_t = long long int;

  bool isOperator(const std::string& token);
  int getPriority(const std::string& token);

  lli_t add(const lli_t& lhs, const lli_t& rhs);
  lli_t sub(const lli_t& lhs, const lli_t& rhs);
  lli_t mult(const lli_t& lhs, const lli_t& rhs);
  lli_t div(const lli_t& lhs, const lli_t& rhs);
  lli_t mod(const lli_t& lhs, const lli_t& rhs);
  lli_t bitOr(const lli_t& lhs, const lli_t& rhs);

  void getInfix(std::istream& in, Stack< Queue< std::string > >& expressions);
  void convertInfToPost(const Queue< std::string >& infix, Queue< std::string >& postfix);
  std::string calculate(const Queue< std::string >& postfix);
}

#endif
