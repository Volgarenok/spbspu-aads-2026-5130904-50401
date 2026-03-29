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

size_t hvostov::getPriority(const std::string& operation)
{
  if (operation == "<<") {
    return 3;
  } else if (operation == "*" || operation == "/") {
    return 2;
  } else if (operation == "+" || operation == "-" || operation == "%") {
    return 1;
  }
  return 0;
}

hvostov::Queue< std::string > hvostov::getPostfix(Queue< std::string >& infix)
{
  Stack< std::string > operations;
  Queue< std::string > posfix;
  while (!infix.empty()) {
    std::string curr = infix.drop();
    if (isNumber(curr)) {
      posfix.push(curr);
    } else if (isSupportedOperand(curr)) {
      size_t priority = getPriority(curr);
      if (priority > 0) {
        while (!operations.empty() &&
            operations.top() != "(" &&
            (getPriority(operations.top()) >= priority)) {
          posfix.push(operations.drop());
        }
        operations.push(curr);
      } else if (curr == "(") {
        operations.push(curr);
      } else if (curr == ")") {
        while (!operations.empty() && operations.top() != "(") {
          posfix.push(operations.drop());
        }
        if (!operations.empty() && operations.top() == "(") {
          operations.drop();
        } else {
          throw std::logic_error("Mismatched parentheses!");
        }
      }
    } else {
      throw std::logic_error("Unsupported operation: " + curr + "!");
    }
  }
  while (!operations.empty()) {
    posfix.push(operations.drop());
  }
  return posfix;
}

long long int hvostov::calculate(long long int left, const std::string& operation, long long int right)
{
  if (operation == "+") {
    return addWithOverflowCheck(left, right);
  } else if (operation == "-") {
    return subtractWithOverflowCheck(left, right);
  } else if (operation == "*") {
    return multiplyWithOverflowCheck(left, right);
  } else if (operation == "/") {
    return divideWithOverflowCheck(left, right);
  } else if (operation == "%") {
    return moduloWithOverflowCheck(left, right);
  } else if (operation == "<<") {
    return shiftLeftWithOverflowCheck(left, right);
  } else {
    throw std::logic_error("Unknown operation: " + operation);
  }
}

long long int hvostov::evaluatePostfix(Queue< std::string >& postfix)
{
  Stack< long long int > values;
  while (!postfix.empty()) {
    std::string token = postfix.drop();
    if (isNumber(token)) {
      values.push(std::stoll(token));
    } else {
      if (values.getSize() < 2) {
        throw std::logic_error("Too few numbers!");
      }
      long long int right = values.drop();
      long long int left = values.drop();
      values.push(calculate(left, token, right));
    }
  }
  if (values.getSize() != 1) {
    throw std::logic_error("Too few operands!");
  }
  return values.drop();
}

hvostov::Stack< long long int > hvostov::getResult(std::istream& in)
{
  Stack< long long int > results;
  std::string line;
  while (std::getline(in, line)) {
    if (line.empty()) {
      continue;
    }
    Queue< std::string > infix = getInfix(line);
    Queue< std::string > postfix = getPostfix(infix);
    long long int result = evaluatePostfix(postfix);
    results.push(result);
  }
  return results;
}

void hvostov::printResult(Stack< long long int > result)
{
  if (result.empty()) {
    return;
  }
  bool first = true;
  while (!result.empty()) {
    if (first) {
      std::cout << result.drop();
      first = false;
    } else {
      std::cout << " " << result.drop();
    }
  }
  std::cout << "\n";
}
