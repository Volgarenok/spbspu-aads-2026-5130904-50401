#ifndef MATH_OPERATIONS_HPP
#define MATH_OPERATIONS_HPP

#include <limits>
#include <cerrno>
#include "queue.hpp"
#include "stack.hpp"

namespace donkeev
{
  using llint_t = long long int;
  void readExpression(donkeev::Queue< donkeev::Queue< char > >&, std::istream& input);
  bool isOperator(const char&);
  size_t getPriority(const char&);
  llint_t doOperation(const llint_t&, const llint_t&, const char&);
  void calculate(donkeev::Stack< llint_t >&, donkeev::Queue< donkeev::Queue< char > >&);
  std::ostream& printResult(Stack< llint_t >&, std::ostream&);
}
#endif
