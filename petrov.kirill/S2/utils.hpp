#ifndef UTILS_HPP
#define UTILS_HPP
#include <iostream>
#include "Stack.hpp"

namespace petrov
{
  namespace detail
  {
    size_t prior(char s);
    long long int oper(long long int a, long long int b, char s);
    size_t isOp(char s);
  }
  Stack<long long int> calcStream(std::istream& in);
}
#endif
