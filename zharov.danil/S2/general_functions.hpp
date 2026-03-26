#ifndef GENERAL_FUNCTIONS_HPP
#define GENERAL_FUNCTIOINS_HPP
#include <iostream>
#include "../common/stack.hpp"
#include "../common/queue.hpp"

namespace zharov {
  using ll_t = long long;
  bool isOperand(const std::string & str);
  Queue< std::string > getQueue(const std::string & line);
  Queue< std::string > getPostfix(Queue< std::string > & infix);
  ll_t getResult(Queue< std::string > & postfix);
  int getPriority(const std::string & op);
}

#endif