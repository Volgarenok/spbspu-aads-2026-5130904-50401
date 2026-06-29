#ifndef FUNC_MATH_HPP
#define FUNC_MATH_HPP
#include <string>

#include "queue.hpp"

namespace zubarev
{

  long long (*getOperation(const std::string& op))(long long, long long);
  long long summation(long long oper1, long long oper2);
  long long division(long long oper1, long long oper2);
  long long multiplication(long long oper1, long long oper2);
  long long subtraction(long long oper1, long long oper2);
  long long remainder(long long oper1, long long oper2);
  long long concatenation(long long oper1, long long oper2);
  long long eval(Queue< std::string >& postfixQ);

}
#endif
