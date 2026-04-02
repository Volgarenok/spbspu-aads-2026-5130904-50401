#ifndef UTILS_S2
#define UTILS_S2

#include <iostream>
#include <string>
#include <cstddef>
#include "stack.hpp"
namespace levkin {

  using Operation = int (*)(int, int);

  size_t getNextWord(const std::string& s, size_t start);
  unsigned short priority(char i);
  int toDigit(std::string& s, size_t start, size_t end, bool& wasnotdigit);
  Operation encodeOpOrThrow(std::string& s, size_t start, size_t end);
  Stack< int > parse(std::istream&);

  int add(int a, int b);
  int subtract(int a, int b);
  int multiply(int a, int b);
  int divide(int a, int b);
  int exponent(int a, int b);
  int reminder(int a, int b);

  void applyOp(Stack< int >&, Stack< Operation >&);
  void processOps(
      Stack< int >& nums,
      Stack< Operation >& ops,
      Stack< char >& symbols,
      char currentOp = '\0');
}
#endif
