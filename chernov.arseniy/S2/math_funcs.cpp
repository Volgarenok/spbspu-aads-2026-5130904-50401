#include "math_funcs.hpp"

#include <cctype>
#include <stdexcept>
#include <string>

long long chernov::abs(long long a)
{
  if (a == MIN_LLI) {
    throw std::overflow_error("overflow error");
  } else if (a >= 0) {
    return a;
  } else {
    return -a;
  }
}

long long chernov::add(long long a, long long b)
{
  if (a > 0 && b > 0 && a > MAX_LLI - b) {
    throw std::overflow_error("overflow error");
  } else if (a < 0 && b < 0 && a < MIN_LLI - b) {
    throw std::underflow_error("underflow error");
  } else {
    return a + b;
  }
}

long long chernov::sub(long long a, long long b)
{
  if (a > 0 && b < 0 && a > MAX_LLI + b) {
    throw std::overflow_error("overflow error");
  } else if (a < 0 && b > 0 && a < MIN_LLI + b) {
    throw std::underflow_error("underflow error");
  } else {
    return a - b;
  }
}

long long chernov::mod(long long a, long long b)
{
  if (b == 0) {
    throw std::logic_error("division by zero");
  } else {
    long long r = a % b;
    r += (r < 0) ? b : 0;
    return r;
  }
}

long long chernov::div(long long a, long long b)
{
  if (b == 0) {
    throw std::logic_error("division by zero");
  } else if (a == MIN_LLI && b == -1) {
    throw std::overflow_error("overflow error");
  } else {
    return a / b;
  }
}

long long chernov::mul(long long a, long long b)
{
  if (a == 0 || b == 0) {
    return 0;
  } else if ((a > 0 && b > 0 && a > MAX_LLI / b) || (a < 0 && b < 0 && a < MAX_LLI / b)) {
    throw std::overflow_error("overflow_error");
  } else if ((a > 0 && b < 0 && b < MIN_LLI / a) || (a < 0 && b > 0 && a < MIN_LLI / b)) {
    throw std::underflow_error("underflow error");
  } else {
    return a * b;
  }
}

long long chernov::gcd(long long a, long long b)
{
  a = abs(a);
  b = abs(b);
  while (a > 0 && b > 0) {
    if (a > b) {
      a = a % b;
    } else {
      b = b % a;
    }
  }
  return a + b;
}

long long chernov::lcm(long long a, long long b)
{
  if (a == 0 || b == 0) {
    return 0;
  }
  long long ab_gcd = gcd(a, b);
  long long a_div = a / ab_gcd;
  return abs(mul(a_div, b));
}

bool chernov::isOperator(const std::string & str)
{
  for (size_t i = 0; i < count_operators; ++i) {
    if (str == operators[i]) {
      return true;
    }
  }
  return false;
}

bool chernov::isOperand(const std::string & str)
{
  if (str == "" || str == "-") {
    return false;
  }
  size_t i = 0;
  i += str[0] == '-';
  while (str[i] != '\0') {
    if (!std::isdigit(str[i])) {
      return false;
    }
    ++i;
  }
  return true;
}

size_t chernov::getPriority(const std::string & oper)
{
  if (oper == "lcm") {
    return 3;
  } else if (oper == "*" || oper == "/" || oper == "%") {
    return 2;
  } else if (oper == "+" || oper == "-") {
    return 1;
  } else {
    return 0;
  }
}
