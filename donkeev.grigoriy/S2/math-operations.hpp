#ifndef MATH_OPERATIONS_HPP
#define MATH_OPERATIONS_HPP

#include "queue.hpp"
#include "stack.hpp"
#include <limits>

using llint_t = long long int;

namespace donkeev
{
  void readFromFile(donkeev::Queue< donkeev::Queue< char > >&, const char*);
  void readFromTerminal(donkeev::Queue< donkeev::Queue< char > >&, std::istream&);
  bool isNumber(const donkeev::Queue< char >& , llint_t &);
  bool isOperator(const char&);
  size_t getPriority(const char&);
  llint_t doOperation(const llint_t&, const llint_t&, const char&);
  void calculate(donkeev::Stack< llint_t >&, donkeev::Queue< donkeev::Queue< char > >&);
  std::ostream& printResult(Stack< llint_t >&, std::ostream&);
}
#endif
