#ifndef MATH_FUNCTIONS
#define MATH_FUNCTIONS
#include <iostream>
#include "queue.hpp"
#include "stack.hpp"
#include <string>
namespace malashenko
{
  using lli_t = long long int;
  void getInfixData(std::istream& in, Queue< std::string >& infixData);
  void converInfixToPostfix(const Queue< std::string >& infixData, Queue< std::string >& PostfixData);
  lli_t calculate(const Queue< std::string >& PostfixData);
  bool isOperand(const std::string& symbol);
  size_t getPriority(const std::string& symbol);
}

#endif