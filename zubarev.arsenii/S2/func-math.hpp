#ifndef FUNC_MATH_HPP
#define FUNC_MATH_HPP
#include "all.hpp"
#include <iosfwd>
namespace zubarev
{
  using ll_int = long long int;
  ll_int summation(ll_int oper1, ll_int oper2);
  ll_int division(ll_int oper1, ll_int oper2);
  ll_int multiplication(ll_int oper1, ll_int oper2);
  ll_int subtraction(ll_int oper1, ll_int oper2);
  ll_int remainder(ll_int oper1, ll_int oper2);
  ll_int concatenation(ll_int oper1, ll_int oper2);
  ll_int fromStrToNum(std::string& str);
  ll_int eval(Queue< std::string >& postfixQ);
}
#endif
