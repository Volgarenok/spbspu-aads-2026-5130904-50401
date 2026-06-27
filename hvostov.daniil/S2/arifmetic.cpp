#include "arifmetic.hpp"
#include <stdexcept>
#include "arifmetic_operations.hpp"
#include <stack.hpp>

bool hvostov::detail::isOperator(const std::string& token)
{
  const std::string operators[] = {"<<", "+", "-", "%", "*", "/"};
  for (size_t i = 0; i < 6; i++) {
    if (token == operators[i]) {
      return true;
    }
  }
  return false;
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
  if (!element.empty()) {
    infix.push(element);
  }
  return infix;
}

size_t hvostov::detail::getPriority(const std::string& operation)
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
  Queue< std::string > postfix;

  while (!infix.empty()) {
    std::string curr = infix.front();
    infix.pop();
    if (curr == "(") {
      operations.push(curr);
    } else if (curr == ")") {
      while (!operations.empty() && operations.top() != "(") {
        postfix.push(operations.top());
        operations.pop();
      }
      if (!operations.empty() && operations.top() == "(") {
        operations.pop();
      } else {
        throw std::logic_error("Mismatched parentheses!");
      }
    } else if (detail::isOperator(curr)) {
      size_t priority = detail::getPriority(curr);
      while (!operations.empty() && operations.top() != "(" && detail::getPriority(operations.top()) >= priority) {
        postfix.push(operations.top());
        operations.pop();
      }
      operations.push(curr);
    } else {
      postfix.push(curr);
    }
  }

  while (!operations.empty()) {
    postfix.push(operations.top());
    operations.pop();
  }

  return postfix;
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
    std::string token = postfix.front();
    postfix.pop();
    if (detail::isOperator(token)) {
      if (values.size() < 2) {
        throw std::logic_error("Too few numbers!");
      }
      long long int right = values.top();
      values.pop();
      long long int left = values.top();
      values.pop();
      values.push(calculate(left, token, right));
    } else {
      values.push(std::stoll(token));
    }
  }

  if (values.size() != 1) {
    throw std::logic_error("Invalid expression!");
  }

  return values.top();
}

long long int hvostov::calculateMathExpression(const std::string& line)
{
  Queue< std::string > infix = getInfix(line);
  Queue< std::string > postfix = getPostfix(infix);
  long long int result = evaluatePostfix(postfix);
  return result;
}
