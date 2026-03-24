#ifndef FUNC_STACK_QUEUE_HPP
#define FUNC_STACK_QUEUE_HPP
#include "all.hpp"
#include <iosfwd>
namespace zubarev
{
  using ll_int = long long int;
  std::string input(std::istream& in, bool& error);
  Queue< std::string > fromStrToQueue(std::string& str);
  bool isdigit(const std::string& str);
  size_t getPriority(const std::string& oper);
  Queue< std::string > fromInfixToPostfix(Queue< std::string >& infixQ);
  void output(std::istream& in);
}

#endif
