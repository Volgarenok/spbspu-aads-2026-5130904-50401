#include <iostream>
#include "general_functions.hpp"
#include "math_functions.hpp"

bool zharov::isOperand(const std::string & str)
{
  std::string signs[] = {"+", "-", "/", "*", "%", "<<", "(", ")"};
  for (size_t i = 0; i < 8; ++i) {
    if (str == signs[i]) {
      return false;
    }
  }
  return true;
}

zharov::Queue< std::string > zharov::getQueue(const std::string & line)
{
  Queue< std::string > res;
  std::string curr;
  for (size_t i = 0; line[i] != '\0'; ++i) {
    if (line[i] == ' ') {
      res.push(curr);
      curr.clear();
    } else {
      curr.push_back(line[i]);
    }
  }
  if (curr != "") {
    res.push(curr);
  }
  return res;
}

int zharov::getPriority(const std::string & op)
{
  if (op == "<<") {
    return 3;
  }
  if (op == "*" || op == "/") {
    return 1;
  }
  return 2;
}

zharov::Queue< std::string > zharov::getPostfix(Queue< std::string > & infix)
{
  Stack< std::string > stack;
  Queue< std::string > res;
  while (!infix.empty()) {
    std::string curr = infix.drop();
    if (isOperand(curr)) {
      res.push(infix.drop());
    } else if (curr == "(") {
      stack.push(infix.drop());
    } else if (curr == ")") {
      while (stack.empty() && stack.top() != "(") {
        res.push(stack.drop());
      }
      stack.drop();
    } else {
      while (!stack.empty() && stack.top() != "(") {
        if (getPriority(curr) > getPriority(stack.top())) {
          res.push(stack.drop());
        } else {
          break;
        }
      }
    }
  }
  return res;
}
