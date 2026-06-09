#ifndef GENERAL_FUNCS_HPP
#define GENERAL_FUNCS_HPP

#include <string>

#include <stack.hpp>
#include <queue.hpp>

namespace chernov {
  class MathExpression {
  public:
    size_t size() const noexcept;
    void processLine(const std::string & line);
    long long calculateMathExpression();
  private:
    Queue< std::string > queue_;
    void executeOperation(Stack< long long > & result, const std::string & oper);
  };
}

#endif
