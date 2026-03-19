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
  ll_int summation(ll_int oper1, ll_int oper2);
  ll_int division(ll_int oper1, ll_int oper2);
  ll_int multiplication(ll_int oper1, ll_int oper2);
  ll_int subtraction(ll_int oper1, ll_int oper2);
  ll_int remainder(ll_int oper1, ll_int oper2);
  ll_int concatenation(ll_int oper1, ll_int oper2);
  ll_int eval(Queue< std::string >& postfixQ);
  ll_int fromStrToNum(std::string& str);

}

#endif
