#ifndef UTILS_HPP
#define UTILS_HPP
#include <iostream>
#include "../common/Stack.hpp"

namespace petrov
{
  size_t prior(char s);
  long long int oper(long long int a, long long int b, char s);
  petrov::Stack<long long int> calcStream(std::istream& in);
}
#endif
