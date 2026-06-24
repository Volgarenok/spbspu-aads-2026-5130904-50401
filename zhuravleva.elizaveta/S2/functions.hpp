#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <string>
#include "operations.hpp"
#include "queue.hpp"

namespace zhuravleva
{
  bool isOperator(const std::string& op);
  int priority(const std::string& op);
  Queue< std::string > infToPostfix(const std::string& line);
  value_t applyBinary(const std::string& op, value_t a, value_t b);
  value_t calcPostfix(Queue< std::string > line);
}

#endif
