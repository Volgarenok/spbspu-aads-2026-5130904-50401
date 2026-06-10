#include "math.hpp"

bool madieva::isOperator(const std::string & token)
{
  return token == "+" ||
    token == "-" ||
    token == "*" ||
    token == "/" ||
    token == "%" ||
    token == "gcd";
}

long long madieva::subtract(long long left, long long right)
{
  if ((right > 0 && left < MIN + right) ||
      (right < 0 && left > MAX + right)) {
    throw std::runtime_error("Overflow");
  }
  return left - right;
}

long long madieva::multiply(long long left, long long right)
{
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

long long madieva::divide(long long left, long long right)
{
  if (right == 0) {
    throw std::runtime_error("Division by zero");
  }
  return left / right;
}

long long madieva::modulo(long long left, long long right)
{
  if (right == 0) {
    throw std::runtime_error("Modulo by zero");
  }
  long long result = left % right;
  if (result < 0) {
    result += right;
  }
  return result;
}

long long madieva::add(long long left, long long right)
{
  if ((right > 0 && left > MAX - right) ||
      (right < 0 && left < MIN - right)) {
    throw std::runtime_error("Overflow");
  }
  return left + right;
}

long long madieva::computeGcd(long long a, long long b)
{
  while (b != 0) {
    long long temp = b;
    b = a % b;
    a = temp;
  }
  if (a < 0) {
    a *= (-1);
  }
  return a;
}

long long madieva::calculateOperation(const std::string & op, long long left, long long right)
{
  if (op == "+") {
    return add(left, right);
  }
  if (op == "-") {
    return subtract(left, right);
  }
  if (op == "*") {
    return multiply(left, right);
  }
  if (op == "/") {
    return divide(left, right);
  }
  if (op == "%") {
    return modulo(left, right);
  }
  if (op == "gcd") {
    return computeGcd(left, right);
  }
  throw std::runtime_error("Unknown operator");
}

void madieva::evaluateExpression(madieva::Queue< std::string > & post, madieva::Stack< long long > & res)
{
  madieva::Stack< long long > temp;
  while (!post.empty()) {
    const std::string token = post.front();
    if (isOperator(token)) {
      if (temp.size() < 2) {
        throw std::runtime_error("Invalid expression");
      }
      const long long right = temp.top();
      temp.pop();
      const long long left = temp.top();
      temp.pop();
      temp.push(calculateOperation(token, left, right));
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
