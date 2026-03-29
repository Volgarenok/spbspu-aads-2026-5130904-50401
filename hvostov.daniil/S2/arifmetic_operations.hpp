#ifndef ARIFMETIC_OPERATIONS_HPP
#define ARIFMETIC_OPERATIONS_HPP

#include <limits>
#include <stdexcept>
#include <climits>

namespace hvostov {
  constexpr long long int LLMAX = std::numeric_limits< long long int >::max();
  constexpr long long int LLMIN = std::numeric_limits< long long int >::min();
  long long int addWithOverflowCheck(long long int left, long long int right);
  long long int subtractWithOverflowCheck(long long int left, long long int right);
  long long int multiplyWithOverflowCheck(long long int left, long long int right);
  long long int divideWithOverflowCheck(long long int left, long long int right);
  long long int moduloWithOverflowCheck(long long int left, long long int right);
  long long int shiftLeftWithOverflowCheck(long long int left, long long int right);
}

#endif
