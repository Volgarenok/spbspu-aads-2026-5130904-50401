#include "math_funcs.hpp"

const lli_t MAX = std::numeric_limits< kuznetsov::lli_t >::max();
const lli_t MIN = std::numeric_limits< kuznetsov::lli_t >::min();

bool kuznetsov::isOperand(const std::string& c)
{
  std::string operators[] = {"+", "-", "*", "/", "%", ">>"};
  for (size_t i = 0; i < 6; ++i) {
    if (c == operators[i]) {
      return false;
    }
  }
  return true;
}

size_t kuznetsov::getPriority(const std::string& c)
{
  if (c == ">>") {
    return 0;
  } else if (c == "+" || c == "-") {
    return 1;
  } else {
    return 2;
  }
}

kuznetsov::lli_t kuznetsov::add(const lli_t& a, const lli_t& b)
{
  if (a > 0 && b > 0 && a > MAX - b) {
    throw std::overflow_error("Add overflow");
  } else if (a < 0 && b < 0 && a < MIN - b) {
    throw std::overflow_error("Add overflow");
  }
  return a + b;
}

kuznetsov::lli_t kuznetsov::sub(const lli_t& a, const lli_t& b)
{
  if (a < 0 && b > 0 && a < MIN + b) {
    throw std::overflow_error("Sub overflow");
  } else if (a > 0 && b < 0 && a < MAX + b) {
    throw std::overflow_error("Sub overflow");
  }
  return a - b;
}

kuznetsov::lli_t kuznetsov::mul(const lli_t& a, const lli_t& b)
{
  if (!a || !b) {
    return 0;
  }
  if (a > 0 && b > 0 && a > MAX / b) {
    throw std::overflow_error("Multiply overflow");
  } else if (a < 0 && b < 0 && a > MAX / b) {
    throw std::overflow_error("Multiply overflow");
  } else if (a > 0 && b < 0 && b < MIN / a) {
    throw std::overflow_error("Multiply overflow");
  } else if (a < 0 && b > 0 && a < MIN / b) {
    throw std::overflow_error("Multiply overflow");
  }
  return a * b;
}

kuznetsov::lli_t kuznetsov::div(const lli_t& a, const lli_t& b)
{
  if (b == 0) {
    throw std::logic_error("Dont div by zero");
  }
  if (a == MIN && b == -1) {
    throw std::overflow_error("Div overflow");
  }
  return a / b;
}

kuznetsov::lli_t kuznetsov::mod(const lli_t& a, const lli_t& b)
{
}

kuznetsov::lli_t kuznetsov::bitShiftToRight(const lli_t& a, const lli_t& b)
{
}
