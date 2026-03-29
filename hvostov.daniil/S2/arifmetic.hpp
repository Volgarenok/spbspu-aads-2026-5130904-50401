#ifndef ARIFMETIC_HPP
#define ARIFMETIC_HPP

#include <cstddef>
#include <string>
#include <istream>
#include <stack.hpp>
#include <queue.hpp>

namespace hvostov {
  Queue< std::string > getInfix(const std::string& expression);
  bool isSupportedOperand(const std::string& operand);
  bool isNumber(const std::string& num);
  size_t getPriority(const std::string& operation);
  Queue< std::string > getPostfix(Queue< std::string >& infix);
  long long int evaluatePostfix(Queue< std::string >& postfix);
  Stack< long long int >getResult(std::istream& in);
  void printResult(Stack< long long int > result);
  long long int calculate(long long int left, const std::string& operation, long long int right);
}

#endif
