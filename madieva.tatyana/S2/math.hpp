#ifndef MATH_HPP
#define MATH_HPP
#include "stack.hpp"
#include "queue.hpp"
#include <fstream>
#include <limits>

const long long MAX = std::numeric_limits<long long>::max();
const long long MIN = std::numeric_limits<long long>::min();

long long gcd(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

long long calculation(const std::string & op, madieva::Stack< long long > & temp) {
  if (temp.empty()) {
    throw std::runtime_error("Empty expression");
  }
  long long right = temp.top();
  temp.pop();
  if (temp.empty()) {
    throw std::runtime_error("Empty expression");
  }
  long long left = temp.top();
  temp.pop();
  if (op == "+") {
    if ((right > 0 && left > MAX - right) ||
      (right < 0 && left < MIN - right)) {
      throw std::runtime_error("Overflow");
    }
    return left + right;
  }
  if (op == "-") {
    if ((right > 0 && left < MIN + right) ||
      (right < 0 && left > MAX + right)) {
      throw std::runtime_error("Overflow");
    }
    return left - right;
  }
  if (op == "*") {
    if (left != 0 && right != 0) {
      if ((left > 0 && right > 0 && left > MAX / right) ||
        (left > 0 && right < 0 && right < MIN / left) ||
        (left < 0 && right > 0 && left < MIN / right) ||
        (left < 0 && right < 0 && left > MAX / right)) {
        throw std::runtime_error("Overflow");
      }
    }
    return left * right;
  }
  if (op == "/") {
    if (right == 0) {
      throw std::runtime_error("Division by zero");
    }
    return left / right;
  }
  if (op == "%") {
    if (right == 0) {
      throw std::runtime_error("Modulo by zero");
    }
    long long result = left % right;
    if (result < 0) {
      result += right;
    }
    return result;
  }
  if (op == "g") {
    return gcd(left, right);
  }
  throw std::runtime_error("Unknown operator");
}

void math(madieva::Queue< std::string > & post, madieva::Stack< long long > & res) {
  madieva::Stack< long long > temp;
  while (!post.empty()) {
    std::string token = post.front();
    if (token == "+" ||
      token == "-" ||
      token == "*" ||
      token == "/" ||
      token == "%" ||
      token == "g") {
      temp.push(calculation(token, temp));
    } else {
      temp.push(std::stoll(token));
    }
    post.pop();
  }
  if (temp.size() != 1) {
    throw std::runtime_error("Invalid expression");
  }
  res.push(temp.top());
}

#endif
