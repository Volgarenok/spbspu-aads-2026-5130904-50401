#ifndef MATH_FUNCS_HPP
#define MATH_FUNCS_HPP
#include <cstddef>
#include <string>
#include "queue.hpp"
#include "stack.hpp"

namespace kondrat
{
  using ll = long long;

  size_t getOperatorType(const std::string & token);
  bool isOperator(const std::string & token);
  size_t getPriority(const std::string & token);

  Queue< std::string > tokenize(const std::string & str);
  Queue< std::string > infixToPostfix(Queue< std::string > & before);
}

#endif