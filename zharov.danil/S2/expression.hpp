#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP
#include <iostream>
#include <queue.hpp>
#include <stack.hpp>

namespace zharov
{
  namespace detail
  {
    Queue< std::string > getQueue(const std::string& line);
    Queue< std::string > getPostfix(Queue< std::string >& infix);
    long long calculate(Queue< std::string >& postfix);
  }

  struct Expression
  {
    Expression(const std::string& line);
    long long calculate();

  private:
    Queue< std::string > postfix_;
  };

}

#endif
