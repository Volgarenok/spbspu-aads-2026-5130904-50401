#ifndef UTILS_S2
#define UTILS_S2

#include <iostream>
#include <string>
#include <cstddef>
#include "stack.hpp"

namespace levkin {
  using Operation = long long (*)(long long, long long);

  extern const long long MAX;
  extern const long long MIN;

  size_t getNextWord(const std::string& s, size_t start);
  unsigned short priority(char i);
  Operation encodeOpOrThrow(const std::string& s);
  Stack< long long > parse(std::istream& in);

  long long add(long long a, long long b);
  long long subtract(long long a, long long b);
  long long multiply(long long a, long long b);
  long long divide(long long a, long long b);
  long long exponent(long long a, long long b);
  long long reminder(long long a, long long b);

  void applyOp(Stack< long long >& nums, Stack< Operation >& ops);
  void processOps(
      Stack< long long >& nums,
      Stack< Operation >& ops,
      Stack< char >& symbols,
      char currentOp = '\0');
}

#endif