#ifndef MATH_FUNCTIONS
#define MATH_FUNCTIONS
#include <iostream>
#include "queue.hpp"
#include "stack.hpp"
#include <string>
#include <limits>
namespace malashenko
{
  using lli_t = long long int;
  using func_t = lli_t(*)(const lli_t&, const lli_t&);
  void getInfixData(std::istream& in, Stack< Queue< std::string > >& infixData);
  void converInfixToPostfix(const Queue< std::string >& infixDataOriginal, Queue< std::string >& PostfixData);
  std::string calculate(const Queue< std::string >& PostfixDataOriginal);
  bool isOperand(const std::string& symbol);

  size_t getPriority(const std::string& symbol);
  lli_t addition(const lli_t& a, const lli_t& b);
  lli_t subtraction(const lli_t& a, const lli_t& b);
  lli_t multiplication(const lli_t& a, const lli_t& b);
  lli_t division(const lli_t& a, const lli_t& b);
  lli_t concatenation(const lli_t & a, const lli_t &b);
  lli_t modulo(const lli_t & a, const lli_t &b);

  size_t getIndex(const std::string& sign, const List< std::string >& funcs);
}

#endif
