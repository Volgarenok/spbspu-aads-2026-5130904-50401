#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <string>
#include "queue.hpp"

namespace tarasenko
{
  enum class TokenType
  {
    num,
    operation,
    left_parenthe,
    right_parenthe
  };
  struct Token
  {
    TokenType type;
    long long value;
    char operation;
    int priority;
  };
  long long calculate(std::string line);
  tarasenko::Queue< Token > stringToQueue(const std::string& line);
  long long getNumber(const std::string& line, size_t& pos);
}

#endif
