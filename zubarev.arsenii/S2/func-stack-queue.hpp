#ifndef FUNC_STACK_QUEUE_HPP
#define FUNC_STACK_QUEUE_HPP
#include "all.hpp"
#include <iosfwd>
namespace zubarev
{
  using ll_int = long long int;
  std::string input(std::istream& in, bool& error);
  Queue< std::string > fromStrToQueue(std::string str);
  bool isdigit(const std::string& str);
  size_t getPriority(const std::string& oper);
  Queue< std::string > fromInfixToPostfix(Queue< std::string > infixQ);
  ll_int summation(size_t oper1, size_t oper2);
  ll_int division(size_t oper1, size_t oper2);
  ll_int multiplication(size_t oper1, size_t oper2);
  ll_int subtraction(size_t oper1, size_t oper2);
  ll_int remainder(size_t oper1, size_t oper2);
  ll_int concatenation(size_t oper1, size_t oper2);
  ll_int evil(Queue< std::string > postfixQ);
  size_t fromStrToNum(std::string str);

}

#endif
