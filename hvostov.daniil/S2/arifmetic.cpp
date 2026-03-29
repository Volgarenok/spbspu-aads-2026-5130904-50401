#include "arifmetic.hpp"
#include "arifmetic_operations.hpp"
#include <iostream>
#include <cctype>
#include <stdexcept>

bool hvostov::isSupportedOperand(const std::string& operand)
{
  std::string supportedOperands[] = {"<<", "+", "-", "%", "*", "/", "(", ")"};
  for (size_t i = 0; i < 8; i++) {
    if (operand == supportedOperands[i]) {
      return true;
    }
  }
  return false;
}

bool hvostov::isNumber(const std::string& num)
{
  if (num.empty() || num == "-") {
    return false;
  }
  for (size_t i = 0; i < num.length(); i++) {
    if (!isdigit(num[i])) {
      if (i == 0 && num[i] == '-') {
        continue;
      } else {
        return false;
      }
    }
  }
  return true;
}

hvostov::Queue< std::string > hvostov::getInfix(const std::string& expression)
{
  Queue< std::string > infix;
  std::string element;
  for (size_t i = 0; i < expression.length(); i++) {
    if (expression[i] == ' ') {
      if (!element.empty()) {
        infix.push(element);
        element.clear();
      }
    } else {
      element.push_back(expression[i]);
    }
  }
  if (element != "") {
    infix.push(element);
  }
  return infix;
}
