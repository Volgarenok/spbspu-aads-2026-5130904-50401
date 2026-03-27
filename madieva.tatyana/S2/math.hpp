#ifndef MATH_HPP
#define MATH_HPP
#include "stack.hpp"
#include "queue.hpp"
#include <fstream>

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int calculation(const std::string & op, madieva::Stack< int > & temp) {
  if (temp.empty()) {
    throw std::runtime_error("Empty expression");
  }
  int right = temp.top();
  temp.pop();
  if (temp.empty()) {
    throw std::runtime_error("Empty expression");
  }
  int left = temp.top();
  temp.pop();
    
  if (op == "+") {
    return left + right;
  }
  if (op == "-") {
    return left - right;
  }
  if (op == "*") {
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
    return left % right;
  }
  if (op == "g") {
    return gcd(left, right);
  }
  
  throw std::runtime_error("Unknown operator");
}

void math(madieva::Queue< std::string > & post, madieva::Stack< int > & res) {
  madieva::Stack< int > temp;
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
      temp.push(std::stoi(token));
    }
    post.pop();
  }
  if (temp.size() != 1) {
    throw std::runtime_error("Invalid expression");
  }
  res.push(temp.top());
}

#endif
