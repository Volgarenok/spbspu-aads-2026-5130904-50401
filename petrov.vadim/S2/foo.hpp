#ifndef FOO_HPP
#define FOO_HPP

#include <string>
#include <cstddef>
#include <istream>
#include "stack.hpp"
#include "queue.hpp"

namespace petrov
{
  bool isOperator(const std::string& token);
  size_t getPriority(const std::string& op);
  size_t reverseNumber(size_t num);
  bool isNumber(const std::string& token);
  void getInfix(const std::istream& in, Stack< Queue < std::string > >& data);
  void infixToPostfix(Stack< Queue < std::string > >& data, Stack< Queue < std::string > >& res);
}

#endif
