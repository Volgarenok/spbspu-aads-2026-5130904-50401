#include <stdexcept>
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
      res.push(curr);
    } else if (curr == "(") {
      stack.push(curr);
    } else if (curr == ")") {
      while (!stack.empty() && stack.top() != "(") {
        res.push(stack.drop());
      }
      if (!stack.empty()) {
        stack.drop();
      } else {
        throw std::logic_error("Bad sign (");
      }
    } else {
      while (!stack.empty() && stack.top() != "(") {
        if (getPriority(curr) >= getPriority(stack.top())) {
          res.push(stack.drop());
        } else {
          break;
        }
      }
      stack.push(curr);
    }
  }
  while (!stack.empty()) {
    if (stack.top() != "(") {
      res.push(stack.drop());
    } else {
      throw std::logic_error("Bad sign (");
    }
  }
  return res;
}

zharov::ll_t zharov::calculate(Queue< std::string > & postfix)
{
  Stack< ll_t > temp;
  while (!postfix.empty()) {
    std::string curr = postfix.drop();
    if (!isOperand(curr)) {
      if (temp.size() < 2) {
        throw std::logic_error("Not enough operands for operator: " + curr);
      }
      else {
        ll_t b = temp.drop();
        ll_t a = temp.drop();
        if (curr == "+") {
          temp.push(add(a, b));
        } else if (curr == "-") {
          temp.push(sub(a, b));
        } else if (curr == "*") {
          temp.push(mul(a, b));
        } else if (curr == "/") {
          temp.push(div(a, b));
        } else if (curr == "%") {
          temp.push(mod(a, b));
        } else if (curr == "<<") {
          temp.push(bitShiftLeft(a, b));
        } else {
          throw std::logic_error("Unknown operator" + curr);
        }
      }
    } else {
      temp.push(std::stoll(curr));
    }
  }
  if (temp.size() != 1) {
    throw std::logic_error("Not enough operators");
  }
  return temp.drop();
}

std::istream & zharov::getResults(std::istream & in, Stack< ll_t > & results)
{
  std::string line;
  while (!std::getline(in, line).eof()) {
    if (line == "") {
      continue;
    }
    zharov::Queue< std::string> queue = zharov::getQueue(line);
    queue = zharov::getPostfix(queue);
    ll_t result = zharov::calculate(queue);
    results.push(result);
  }
  return in;
}
