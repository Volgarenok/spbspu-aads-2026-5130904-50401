#ifndef MATH_OPERATIONS_HPP
#define MATH_OPERATIONS_HPP

#include "queue.hpp"

namespace donkeev
{
  void readFromFile(donkeev::Queue< donkeev::Queue< char > >&, const char*);
  void readFromTerminal(donkeev::Queue< donkeev::Queue< char > >&, std::istream&);
  void calculate(const char*);
}
#endif
