#ifndef MATH_FUNCTIONS
#define MATH_FUNCTIONS
#include <iostream>
#include "queue.hpp"
#include "stack.hpp"
#include <string>
namespace malashenko
{
  using lli_t = long long int;
  using func_t = lli_t(*)(lli_t, lli_t);
  void getInfixData(std::istream& in, Queue< std::string >& infixData);
  void converInfixToPostfix(Queue< std::string > infixData, Queue< std::string >& PostfixData);
  lli_t calculate(const Queue< std::string >& PostfixData);
  bool isOperand(const std::string& symbol);
  size_t getPriority(const std::string& symbol);

  lli_t addition(lli_t a, lli_t b);
  lli_t subtraction(lli_t a, lli_t b);
  lli_t multiplication(lli_t a, lli_t b);
  lli_t division(lli_t a, lli_t b);

  size_t getIndex(const std::string& sign, const func_t* funcs);

}

#endif