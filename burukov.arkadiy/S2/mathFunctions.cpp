#include "mathFunctions.hpp"
#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

burukov::lli_t burukov::add(burukov::lli_t lhs, burukov::lli_t rhs)
{
  const burukov::lli_t maxVal = std::numeric_limits< burukov::lli_t >::max();
  const burukov::lli_t minVal = std::numeric_limits< burukov::lli_t >::min();
  if ((rhs > 0) && (lhs > maxVal - rhs))
  {
    throw std::overflow_error("Add overflow");
  }
  if ((rhs < 0) && (lhs < minVal - rhs))
  {
    throw std::overflow_error("Add overflow");
  }
  return lhs + rhs;
}

burukov::lli_t burukov::sub(burukov::lli_t lhs, burukov::lli_t rhs)
{
  const burukov::lli_t maxVal = std::numeric_limits< burukov::lli_t >::max();
  const burukov::lli_t minVal = std::numeric_limits< burukov::lli_t >::min();
  if ((rhs > 0) && (lhs < minVal + rhs))
  {
    throw std::overflow_error("Sub overflow");
  }
  if ((rhs < 0) && (lhs > maxVal + rhs))
  {
    throw std::overflow_error("Sub overflow");
  }
  return lhs - rhs;
}

burukov::lli_t burukov::mul(burukov::lli_t lhs, burukov::lli_t rhs)
{
  const burukov::lli_t maxVal = std::numeric_limits< burukov::lli_t >::max();
  const burukov::lli_t minVal = std::numeric_limits< burukov::lli_t >::min();
  if (lhs == 0 || rhs == 0)
  {
    return 0;
  }
  if (lhs == -1)
  {
    if (rhs == minVal)
    {
      throw std::overflow_error("Mul overflow");
    }
    return -rhs;
  }
  if (rhs == -1)
  {
    if (lhs == minVal)
    {
      throw std::overflow_error("Mul overflow");
    }
    return -lhs;
  }
  if (lhs > 0)
  {
    if (rhs > 0)
    {
      if (lhs > maxVal / rhs)
      {
        throw std::overflow_error("Mul overflow");
      }
    }
    else
    {
      if (rhs < minVal / lhs)
      {
        throw std::overflow_error("Mul overflow");
      }
    }
  }
  else
  {
    if (rhs > 0)
    {
      if (lhs < minVal / rhs)
      {
        throw std::overflow_error("Mul overflow");
      }
    }
    else
    {
      if (lhs != 0 && rhs < maxVal / lhs)
      {
        throw std::overflow_error("Mul overflow");
      }
    }
  }
  return lhs * rhs;
}

burukov::lli_t burukov::div(burukov::lli_t lhs, burukov::lli_t rhs)
{
  const burukov::lli_t minVal = std::numeric_limits< burukov::lli_t >::min();
  if (rhs == 0)
  {
    throw std::invalid_argument("Div by 0");
  }
  if ((lhs == minVal) && (rhs == -1))
  {
    throw std::overflow_error("Div overflow");
  }
  return lhs / rhs;
}

burukov::lli_t burukov::mod(burukov::lli_t lhs, burukov::lli_t rhs)
{
  const burukov::lli_t minVal = std::numeric_limits< burukov::lli_t >::min();
  if (rhs == 0)
  {
    throw std::invalid_argument("Mod by 0");
  }
  if ((lhs == minVal) && (rhs == -1))
  {
    throw std::overflow_error("Mod overflow");
  }
  burukov::lli_t result = lhs % rhs;
  if ((result > 0) != (rhs > 0) && result != 0)
  {
    result += rhs;
  }
  return result;
}

burukov::lli_t burukov::bitwiseAnd(burukov::lli_t lhs, burukov::lli_t rhs)
{
  return lhs & rhs;
}

bool burukov::detail::isOperation(const std::string &sym)
{
  return sym == "+" || sym == "-" || sym == "*"
      || sym == "/" || sym == "%" || sym == "&";
}

int burukov::detail::getPriority(const std::string &sym)
{
  if (sym == "&")
  {
    return 1;
  }
  if (sym == "+" || sym == "-")
  {
    return 0;
  }
  if (sym == "*" || sym == "/" || sym == "%")
  {
    return 2;
  }
  return -1;
}

void burukov::detail::convertToPostfix(const burukov::Queue< std::string > &infix,
  burukov::Queue< std::string > &postfix)
{
  burukov::Queue< std::string > infixCopy = infix;
  burukov::Stack< std::string > ops;
  while (!infixCopy.empty())
  {
    const std::string sym = infixCopy.front();
    infixCopy.pop();
    if (sym == "(")
    {
      ops.push(sym);
      continue;
    }
    if (sym == ")")
    {
      while (!ops.empty() && ops.top() != "(")
      {
        postfix.push(ops.top());
        ops.pop();
      }
      if (ops.empty())
      {
        throw std::invalid_argument("Mismatched parentheses");
      }
      ops.pop();
      continue;
    }
    if (!isOperation(sym))
    {
      postfix.push(sym);
    }
    else
    {
      while (!ops.empty() && ops.top() != "(" && getPriority(ops.top()) >= getPriority(sym))
      {
        postfix.push(ops.top());
        ops.pop();
      }
      ops.push(sym);
    }
  }
  while (!ops.empty())
  {
    if (ops.top() == "(")
    {
      throw std::invalid_argument("Mismatched parentheses");
    }
    postfix.push(ops.top());
    ops.pop();
  }
}

burukov::lli_t burukov::detail::applyOperation(const std::string &sym,
  burukov::lli_t lhs, burukov::lli_t rhs)
{
  if (sym == "+")
  {
    return burukov::add(lhs, rhs);
  }
  if (sym == "-")
  {
    return burukov::sub(lhs, rhs);
  }
  if (sym == "*")
  {
    return burukov::mul(lhs, rhs);
  }
  if (sym == "/")
  {
    return burukov::div(lhs, rhs);
  }
  if (sym == "%")
  {
    return burukov::mod(lhs, rhs);
  }
  if (sym == "&")
  {
    return burukov::bitwiseAnd(lhs, rhs);
  }
  throw std::invalid_argument("Unknown operation");
}

burukov::lli_t burukov::detail::calculate(const burukov::Queue< std::string > &postfix)
{
  burukov::Queue< std::string > postfixCopy = postfix;
  burukov::Stack< burukov::lli_t > nums;
  while (!postfixCopy.empty())
  {
    const std::string sym = postfixCopy.front();
    postfixCopy.pop();
    if (!isOperation(sym))
    {
      size_t pos = 0;
      burukov::lli_t num = std::stoll(sym, &pos);
      if (pos != sym.size())
      {
        throw std::invalid_argument("Invalid number");
      }
      nums.push(num);
    }
    else
    {
      if (nums.size() < 2)
      {
        throw std::invalid_argument("Invalid expression");
      }
      burukov::lli_t rhs = nums.top();
      nums.pop();
      burukov::lli_t lhs = nums.top();
      nums.pop();
      burukov::lli_t result = applyOperation(sym, lhs, rhs);
      nums.push(result);
    }
  }
  if (nums.size() != 1)
  {
    throw std::invalid_argument("Invalid expression");
  }
  return nums.top();
}

void burukov::getInfix(std::istream &in, burukov::Stack< burukov::Queue< std::string > > &infix)
{
  char sym = 0;
  std::string current;
  burukov::Queue< std::string > expression;
  while (in.get(sym))
  {
    if (sym == '\n')
    {
      if (!current.empty())
      {
        expression.push(current);
        current.clear();
      }
      if (!expression.empty())
      {
        infix.push(expression);
        while (!expression.empty())
        {
          expression.pop();
        }
      }
    }
    else if (sym == ' ' || sym == '\t')
    {
      if (!current.empty())
      {
        expression.push(current);
        current.clear();
      }
    }
    else
    {
      current += sym;
    }
  }
  if (!current.empty())
  {
    expression.push(current);
  }
  if (!expression.empty())
  {
    infix.push(expression);
  }
}

std::string burukov::evaluateExpression(const burukov::Queue< std::string > &infix)
{
  burukov::Queue< std::string > postfix;
  burukov::detail::convertToPostfix(infix, postfix);
  burukov::lli_t result = burukov::detail::calculate(postfix);
  return std::to_string(result);
}
