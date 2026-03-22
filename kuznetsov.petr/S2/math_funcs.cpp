#include "math_funcs.hpp"

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
}

kuznetsov::lli_t kuznetsov::sub(const lli_t& a, const lli_t& b)
{
}

kuznetsov::lli_t kuznetsov::mul(const lli_t& a, const lli_t& b)
{
}

kuznetsov::lli_t kuznetsov::div(const lli_t& a, const lli_t& b)
{
}

kuznetsov::lli_t kuznetsov::mod(const lli_t& a, const lli_t& b)
{
}

kuznetsov::lli_t kuznetsov::bitShiftToRight(const lli_t& a, const lli_t& b)
{
}
