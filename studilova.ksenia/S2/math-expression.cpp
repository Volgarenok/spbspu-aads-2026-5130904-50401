#include "math-expression.hpp"
#include "stack.hpp"

#include <stdexcept>

bool studilova::isNumber(const std::string& s)
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
    if (s[i] < '0' || s[i] > '9')
    {
      return false;
    }
  }
  return true;
}

bool studilova::isOperator(const std::string& s)
{
  return s == "+" || s == "-" || s == "*" || s == "/" || s == "%" || s == "**";
}

int studilova::getPrecedence(const std::string& op)
{
  if (op == "+" || op == "-")
  {
    return 1;
  }
  if (op == "*" || op == "/" || op == "%")
  {
    return 2;
  }
  if (op == "**")
  {
    return 3;
  }
  return 0;
}

bool studilova::IsRightAssociative(const std::string& op)
{
  return op == "**";
}

long long studilova::add(long long a, long long b)
{
  if (b > 0 && a > studilova::MAX - b)
  {
    throw std::overflow_error("Addition overflow");
  }
  if (b < 0 && a < studilova::MIN - b)
  {
    throw std::overflow_error("Addition underflow");
  }
  return a + b;
}

long long studilova::subtract(long long a, long long b)
{
  if (b > 0 && a < studilova::MIN + b)
  {
    throw std::overflow_error("Subtract underflow");
  }
  if (b < 0 && a > studilova::MAX + b)
  {
    throw std::overflow_error("Subtract overlow");
  }
  return a - b;
}

long long studilova::multiply(long long a, long long b)
{
  if (a == 0 || b == 0)
  {
    return 0;
  }
  if (a > 0)
  {
    if (b > 0 && a > studilova::MAX / b)
    {
      throw std::overflow_error("Multiplication overflow");
    }
    if (b < 0 && b > studilova::MIN / a)
    {
      throw std::overflow_error("Multiplication underflow");
    }
  } else {
    if (b > 0 && a < studilova::MIN / b)
    {
      throw std::overflow_error("Multiplication underflow");
    }
    if (b < 0 && a < studilova::MAX / b)
    {
      throw std::overflow_error("Multiplication overflow");
    }
  }
  return a * b;
}

long long studilova::divide(long long a, long long b)
{
  if (b == 0)
  {
    throw std::runtime_error("Division by zero");
  }
  if (a == studilova::MIN && b == -1)
  {
    throw std::overflow_error("Division overflow");
  }
  return a / b;
}

long long studilova::mod(long long a, long long b)
{
  if (b == 0)
  {
    throw std::runtime_error("Modulo by zero");
  }
  return (a % b + b) % b;
}

long long studilova::power(long long a, long long b)
{
  if (b < 0)
  {
    throw std::runtime_error("Negative exponent");
  }
  long long res = 1;
  while (b--)
  {
    res = studilova::multiply(res, a);
  }
  return res;
}

long long studilova::applyOperator( long long a, long long b, const std::string& op)
{
  if (op == "+")
  {
    return studilova::add(a, b);
  }
  if (op == "-")
  {
    return studilova::subtract(a, b);
  }
  if (op == "*")
  {
    return studilova::multiply(a, b);
  }
  if (op == "/")
  {
    return studilova::divide(a, b);
  }
  if (op == "%")
  {
    return studilova::mod(a, b);
  }
  if (op == "**")
  {
    return studilova::power(a, b);
  }
  throw std::runtime_error("Unknown operator");
}
