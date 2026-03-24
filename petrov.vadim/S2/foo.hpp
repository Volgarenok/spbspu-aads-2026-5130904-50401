#ifndef FOO_HPP
#define FOO_HPP

#include <string>
#include <cstddef>

namespace petrov
{
  bool isOperator(const std::string& token);
  int getPriority(const std::string& op);
  size_t reverseNumber(size_t num);
  bool isNumber(const std::string& token);
}

#endif
