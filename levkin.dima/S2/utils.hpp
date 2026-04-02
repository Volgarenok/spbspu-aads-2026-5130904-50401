#ifndef UTILS_S2
#define UTILS_S2

#include <iostream>
#include <limits>
#include <string>
#include <cstddef>
#include "stack.hpp"

namespace levkin {

  using Operation = long long (*)(long long, long long);
  
  const long long MAX = std::numeric_limits< long long >::max();
  const long long MIN = std::numeric_limits< long long >::min();

  size_t getNextWord(const std::string& s, size_t start);
  unsigned short priority(char i);
  long long toDigit(std::string& s, size_t start, size_t end, bool& wasnotdigit);
  Operation encodeOpOrThrow(std::string& s, size_t start, size_t end);
  Stack< long long > parse(std::istream&);

  long long add(long long a, long long b);
  long long subtract(long long a, long long b);
  long long multiply(long long a, long long b);
  long long divide(long long a, long long b);
  long long exponent(long long a, long long b);
  long long reminder(long long a, long long b);

  void applyOp(Stack< long long >&, Stack< Operation >&);
  void processOps(
      Stack< long long >& nums,
      Stack< Operation >& ops,
      Stack< char >& symbols,
      char currentOp = '\0');
}
#endif
