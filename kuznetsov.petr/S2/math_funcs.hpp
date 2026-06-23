#ifndef MATH_FUNCS_HPP
#define MATH_FUNCS_HPP
#include <cstddef>
#include <limits>
#include <string>
#include <queue.hpp>
#include <stack.hpp>

namespace kuznetsov {
  using lli_t = long long;

  namespace detail {
    const lli_t MAX = std::numeric_limits< lli_t >::max();
    const lli_t MIN = std::numeric_limits< lli_t >::min();
    bool isOperation(const std::string& c);
    size_t getPriority(const std::string& c);
  }

  lli_t add(const lli_t& a, const lli_t& b);
  lli_t sub(const lli_t& a, const lli_t& b);
  lli_t mul(const lli_t& a, const lli_t& b);
  lli_t div(const lli_t& a, const lli_t& b);
  lli_t mod(const lli_t& a, const lli_t& b);
  lli_t bitShiftToRight(const lli_t& a, const lli_t& b);

  struct InfixExpression {
    void pushToken(const std::string& token);
    bool empty() const noexcept;
    lli_t evaluate() const;

  private:
    Queue< std::string > tokens_;
    Queue< std::string > toPostfix() const;
  };

  using stackOfInfixExpression = Stack< InfixExpression >;
  stackOfInfixExpression getExpressions(std::istream & in);
  Queue< lli_t > calculateStackOfInfix(stackOfInfixExpression infix);
}

#endif
