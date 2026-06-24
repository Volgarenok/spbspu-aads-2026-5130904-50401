#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <limits>
#include <string>
#include "queue.hpp"
#include "stack.hpp"

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

  long long calculate(const std::string& line);
  tarasenko::Queue< std::string > stringToQueue(const std::string& line);
  Token stringToToken(const std::string& str);
  bool isOperation(const std::string& str);
  int getPriority(const std::string& str);
  using tokenStack = Stack< Token >;
  using numStack = Stack< long long >;
  void makeTopOperation(tokenStack& operations_stack, numStack& operands_stack);

  long long add(long long a, long long b);
  long long subtract(long long a, long long b);
  long long multiply(long long a, long long b);
  long long divide(long long a, long long b);
  long long mod(long long a, long long b);
  long long rightShift(long long a, long long b);
  const long long MAX = std::numeric_limits< long long >::max();
  const long long MIN = std::numeric_limits< long long >::min();
}

#endif
