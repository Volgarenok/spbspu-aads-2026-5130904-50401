#ifndef UTILS_S2
#define UTILS_S2

#include <iostream>
#include <string>
#include <cstddef>
#include "stack.hpp"
namespace levkin {

  using Operation = size_t (*)(size_t, size_t);

  size_t getNextWord(const std::string& s, size_t start);
  unsigned short priority(char i);
  size_t toDigit(std::string& s, size_t start, size_t end, bool& wasnotdigit);
  Operation encodeOpOrThrow(std::string& s, size_t start, size_t end);
  Stack< size_t > parse(std::istream&);

  size_t add(size_t a, size_t b);
  size_t subtract(size_t a, size_t b);
  size_t multiply(size_t a, size_t b);
  size_t divide(size_t a, size_t b);
  size_t exponent(size_t a, size_t b);
  size_t reminder(size_t a, size_t b);

  void applyOp(Stack< size_t >&, Stack< Operation >&);
  void processOps(
      Stack< size_t >& nums,
      Stack< Operation >& ops,
      Stack< char >& symbols,
      char currentOp = '\0');
}
#endif
