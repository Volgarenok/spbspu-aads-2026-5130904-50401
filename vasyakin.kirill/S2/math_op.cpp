#include "math_op.hpp"
#include "stack.hpp"
#include <limits>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <cmath>

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

long long vasyakin::gcd(long long a, long long b)
{
  a = std::abs(a);
  b = std::abs(b);
  while (b != 0)
  {
    long long temp = b;
    b = a % b;
    a = temp;
  }
  return a;
}

long long vasyakin::lcm(long long a, long long b)
{
  if (a == 0 || b == 0)
  {
    throw std::runtime_error("LCM with zero is undefined");
  }
  long long g = gcd(a, b);
  long long abs_a = std::abs(a);
  long long abs_b = std::abs(b);

  if (abs_a / g > vasyakin::MAX / abs_b)
  {
    throw std::overflow_error("LCM overflow");
  }
  return (abs_a / g) * abs_b;
}

long long vasyakin::sum(long long a, long long b)
{
  if (b > 0 && a > vasyakin::MAX - b)
  {
    throw std::overflow_error("Sum overflow");
  }
  else if (b < 0 && a < vasyakin::MIN - b)
  {
    throw std::overflow_error("Sum underflow");
  }
  return a + b;
}

long long vasyakin::subtract(long long a, long long b)
{
  if (b > 0 && a < vasyakin::MIN + b)
  {
    throw std::overflow_error("Subtract underflow");
  }
  else if (b < 0 && a > vasyakin::MAX + b)
  {
    throw std::overflow_error("Subtract overflow");
  }
  return a - b;
}

long long vasyakin::multiply(long long a, long long b)
{
  if (a == 0 || b == 0)
  {
    return 0;
  }

  if (a > 0)
  {
    if (b > 0 && a > vasyakin::MAX / b)
    {
      throw std::overflow_error("Mult overflow");
    }
    else if (b < 0 && b < vasyakin::MIN / a)
    {
      throw std::overflow_error("Mult underflow");
    }
  }
  else
  {
    if (b > 0 && a < vasyakin::MIN / b)
    {
      throw std::overflow_error("Mult underflow");
    }
    else if (b < 0 && a < vasyakin::MAX / b)
    {
      throw std::overflow_error("Mult overflow");
    }
  }
  return a * b;
}

long long vasyakin::divide(long long a, long long b)
{
  if (b == 0)
  {
    throw std::runtime_error("Division by zero");
  }

  if (a == vasyakin::MIN && b == -1)
  {
    throw std::overflow_error("Division overflow");
  }
  return a / b;
}

long long vasyakin::mod(long long a, long long b)
{
  if (b == 0)
  {
    throw std::runtime_error("Division by zero");
  }
  else
  {
    return (a % b + b) % b;
  }
}

long long vasyakin::calculate(long long a, long long b, const std::string& op)
{
  if (op == "+")
  {
    return vasyakin::sum(a, b);
  }
  if (op == "-")
  {
    return vasyakin::subtract(a, b);
  }
  if (op == "*")
  {
    return vasyakin::multiply(a, b);
  }
  if (op == "/")
  {
    return vasyakin::divide(a, b);
  }
  if (op == "%")
  {
    return vasyakin::mod(a, b);
  }
  if (op == "lcm")
  {
    return vasyakin::lcm(a, b);
  }
  throw std::runtime_error("Unknown operator: " + op);
}

vasyakin::Queue< std::string > vasyakin::infixToPostfix(const std::string& line)
{
  vasyakin::Stack< std::string > opStack;
  vasyakin::Queue< std::string > postfix;

  std::string token;

  for (size_t i = 0; i < line.length(); ++i)
  {
    if (line[i] == ' ')
    {
      if (!token.empty())
      {
        if (vasyakin::isNumber(token))
        {
          postfix.push(token);
        }
        else if (token == "(")
        {
          opStack.push(token);
        }
        else if (token == ")")
        {
          while (!opStack.empty() && opStack.peek() != "(")
          {
            postfix.push(opStack.drop());
          }
          if (opStack.empty())
          {
            throw std::runtime_error("Mismatched parentheses");
          }
          opStack.drop();
        }
        else if (vasyakin::isOperator(token))
        {
          while (!opStack.empty() && opStack.peek() != "(" && vasyakin::getPrecedence(opStack.peek()) >= vasyakin::getPrecedence(token))
          {
            postfix.push(opStack.drop());
          }
          opStack.push(token);
        }
        else
        {
          throw std::runtime_error("Invalid token: " + token);
        }
        token.clear();
      }
    }
    else
    {
      token += line[i];
    }
  }

  if (!token.empty())
  {
    if (vasyakin::isNumber(token))
    {
      postfix.push(token);
    }
    else if (token == "(")
    {
      opStack.push(token);
    }
    else if (token == ")")
    {
      while (!opStack.empty() && opStack.peek() != "(")
      {
        postfix.push(opStack.drop());
      }
      if (opStack.empty())
      {
        throw std::runtime_error("Mismatched parentheses");
      }
      opStack.drop();
    }
    else if (vasyakin::isOperator(token))
    {
      while (!opStack.empty() && opStack.peek() != "(" && vasyakin::getPrecedence(opStack.peek()) >= vasyakin::getPrecedence(token))
      {
        postfix.push(opStack.drop());
      }
      opStack.push(token);
    }
    else
    {
      throw std::runtime_error("Invalid token: " + token);
    }
  }
  while (!opStack.empty())
  {
    std::string op = opStack.drop();
    if (op == "(" || op == ")")
    {
      throw std::runtime_error("Mismatched parentheses");
    }
    postfix.push(op);
  }
  return postfix;
}

long long vasyakin::evaluatePostfix(vasyakin::Queue< std::string >& postfix)
{
  vasyakin::Stack< long long > temp;

  while (!postfix.empty())
  {
    std::string token = postfix.drop();

    if (vasyakin::isNumber(token))
    {
      temp.push(std::stoll(token));
    }
    else if (vasyakin::isOperator(token))
    {
      if (temp.size() < 2)
      {
        throw std::runtime_error("Invalid expression: not enough operands");
      }

      long long b = temp.drop();
      long long a = temp.drop();
      long long res = vasyakin::calculate(a, b, token);
      temp.push(res);
    }
    else
    {
      throw std::runtime_error("Invalid token in evaluation: " + token);
    }
  }

  if (temp.size() != 1)
  {
    throw std::runtime_error("Invalid expression: stack not empty");
  }

  return temp.drop();
}

long long vasyakin::evaluateExpression(const std::string& line)
{
  vasyakin::Queue< std::string > postfix = vasyakin::infixToPostfix(line);
  return vasyakin::evaluatePostfix(postfix);
}
