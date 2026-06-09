#ifndef MATH_OPERATIONS_HPP
#define MATH_OPERATIONS_HPP

#include <limits>
#include <cerrno>
#include "queue.hpp"
#include "stack.hpp"

namespace donkeev
{
  using llint_t = long long int;
  void readFromFile(donkeev::Queue< donkeev::Queue< char > >&, std::ifstream& input);
  void readFromTerminal(donkeev::Queue< donkeev::Queue< char > >&, std::istream&);
  bool isNumber(const donkeev::Queue< char >& , llint_t &);
  bool isOperator(const char&);
  size_t getPriority(const char&);
  llint_t doOperation(const llint_t&, const llint_t&, const char&);
  void calculate(donkeev::Stack< llint_t >&, donkeev::Queue< donkeev::Queue< char > >&);
  std::ostream& printResult(Stack< llint_t >&, std::ostream&);
}
#endif
