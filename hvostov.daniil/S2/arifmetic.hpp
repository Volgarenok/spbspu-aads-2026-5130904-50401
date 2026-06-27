#ifndef ARIFMETIC_HPP
#define ARIFMETIC_HPP

#include <cstddef>
#include <string>
#include <queue.hpp>

namespace hvostov {
  namespace detail {
    bool isOperator(const std::string& token);
    size_t getPriority(const std::string& operation);
  }

  Queue< std::string > getInfix(const std::string& expression);
  Queue< std::string > getPostfix(Queue< std::string >& infix);
  long long int evaluatePostfix(Queue< std::string >& postfix);
  long long int calculate(long long int left, const std::string& operation, long long int right);
  long long int calculateMathExpression(const std::string& line);
}

#endif
