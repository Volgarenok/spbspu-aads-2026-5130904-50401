#include "math_op.hpp"
#include "stack.hpp"
#include <limits>
#include <cstddef>
#include <stdexcept>
#include <string>

bool vasyakin::isNumber(const std::string& s)
{
  if (s.empty())
  {
    return false;
  }

  size_t start = 0;
  
  if (s[start] == '-')
  {
    if (s.length() == 1)
    {
      return false;
    }
    ++start;
  }

  for (size_t i = start; i < s.length(); ++i)
  {
    if (!std::isdigit(static_cast< unsigned char >(s[i])))
    {
      return false;
    }
  }

  return true;
}

bool vasyakin::isOperator(const std::string& s)
{
  if (s.empty())
  {
    return false;
  }

  if (s.length() == 1 && ((s[0] == '-') || (s[0] == '+') || (s[0] == '/') || (s[0] == '*') || (s[0] == '%')))
  {
    return true;
  }
  else if (s.length() == 3)
  {
    std::string correct_str = "lcm";

    if (correct_str == s)
    {
      return true;
    }
  }
  return false;
}

int vasyakin::getPrecedence(const std::string& op)
{
  if (op == "lcm")
  {
    return 3;
  }

  if (op == "*" || op == "/" || op == "%")
  {
    return 2;
  }

  if (op == "+" || op == "-")
  {
    return 1;
  }
  return 0;  
}
