#include "math_funcs.hpp"

#include <stdexcept>

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
    return a % b;
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
  } else if ((a > 0 && b < 0 && a > MIN_LLI / b) || (a < 0 && b > 0 && a < MIN_LLI / b)) {
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
  long long ab_gcd = gcd(a, b);
  long long a_div = a / ab_gcd;
  return mul(a_div, b);
}

bool chernov::isOperator(const std::string & str)
{
  constexpr size_t count_operators = 7;
  const std::string operators[count_operators] = {"+", "-", "*", "/", "%", "lcm"};
  for (size_t i = 0; i < count_operators; ++i) {
    if (str == operators[i]) {
      return true;
    }
  }
  return false;
}
