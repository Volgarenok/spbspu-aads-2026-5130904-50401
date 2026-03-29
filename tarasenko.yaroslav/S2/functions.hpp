#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

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

  long long calculate(std::string line);
  tarasenko::Queue< Token > stringToQueue(const std::string& line);
  long long getNumber(const std::string& line, size_t& pos);
  using tokenStack = Stack< Token >;
  using numStack = Stack< long long >;
  void makeTopOperation(tokenStack& operations_stack, numStack& operands_stack);

  long long add(long long a, long long b);
  long long subtract(long long a, long long b);
  long long multiply(long long a, long long b);
  long long divide(long long a, long long b);
  long long mod(long long a, long long b);
  long long rightShift(long long a, long long b);

  void readStreamAndPrintResults(std::istream& stream);
}

#endif
