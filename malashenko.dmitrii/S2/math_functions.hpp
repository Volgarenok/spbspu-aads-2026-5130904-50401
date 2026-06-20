#ifndef MATH_FUNCTIONS_HPP
#define MATH_FUNCTIONS_HPP
#include <iostream>
#include <string>
#include <limits>
#include "queue.hpp"
#include "stack.hpp"
namespace malashenko
{
  using lli_t = long long int;
  namespace detail
  {

    using func_t = lli_t(*)(const lli_t&, const lli_t&);
    void getInfixData(std::istream& in, Stack< Queue< std::string > >& infixData);
    void converInfixToPostfix(Queue< std::string >& infixData, Queue< std::string >& PostfixData);
    lli_t calculate(Queue< std::string >& PostfixDataOriginal);
    bool isOperation(const std::string& symbol);

    size_t getPriority(const std::string& symbol);
    lli_t addition(const lli_t& a, const lli_t& b);
    lli_t subtraction(const lli_t& a, const lli_t& b);
    lli_t multiplication(const lli_t& a, const lli_t& b);
    lli_t division(const lli_t& a, const lli_t& b);
    lli_t concatenation(const lli_t & a, const lli_t &b);
    lli_t modulo(const lli_t & a, const lli_t &b);

    size_t getIndex(const std::string& sign, const List< std::string >& funcs);
  }
}

#endif
