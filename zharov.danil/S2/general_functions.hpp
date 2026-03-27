#ifndef GENERAL_FUNCTIONS_HPP
#define GENERAL_FUNCTIONS_HPP
#include <iostream>
#include <stack.hpp>
#include <queue.hpp>

namespace zharov {
  using ll_t = long long;
  bool isOperand(const std::string & str);
  Queue< std::string > getQueue(const std::string & line);
  Queue< std::string > getPostfix(Queue< std::string > & infix);
  ll_t calculate(Queue< std::string > & postfix);
  int getPriority(const std::string & op);
  std::istream & getResults(std::istream & in, Stack< ll_t > & results);
}

#endif
