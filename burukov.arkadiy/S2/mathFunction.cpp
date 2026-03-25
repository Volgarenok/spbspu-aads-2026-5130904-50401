#include "mathFunctions.hpp"
#include <cerrno>
#include <cstdlib>
#include <limits>
#include <stdexcept>
#include <string>

bool burukov::isOperand(const std::string &sym)
{
  return sym != "+" && sym != "-" && sym != "*"
      && sym != "/" && sym != "%" && sym != "&"
      && sym != "(" && sym != ")";
}

int burukov::getPriority(const std::string &sym)
{
  if (sym == "&")
  {
    return 0;
  }
  if (sym == "+" || sym == "-")
  {
    return 1;
  }
  if (sym == "*" || sym == "/" || sym == "%")
  {
    return 2;
  }
  return -1;
}

burukov::lli_t burukov::add(const lli_t lhs, const lli_t rhs)
{
  const lli_t maxVal = std::numeric_limits< lli_t >::max();
  const lli_t minVal = std::numeric_limits< lli_t >::min();
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

burukov::lli_t burukov::sub(const lli_t lhs, const lli_t rhs)
{
  const lli_t maxVal = std::numeric_limits< lli_t >::max();
  const lli_t minVal = std::numeric_limits< lli_t >::min();
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

burukov::lli_t burukov::mul(const lli_t lhs, const lli_t rhs)
{
  const lli_t maxVal = std::numeric_limits< lli_t >::max();
  const lli_t minVal = std::numeric_limits< lli_t >::min();
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

burukov::lli_t burukov::div(const lli_t lhs, const lli_t rhs)
{
  const lli_t minVal = std::numeric_limits< lli_t >::min();
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

burukov::lli_t burukov::mod(const lli_t lhs, const lli_t rhs)
{
  const lli_t minVal = std::numeric_limits< lli_t >::min();
  if (rhs == 0)
  {
    throw std::invalid_argument("Mod by 0");
  }
  if ((lhs == minVal) && (rhs == -1))
  {
    throw std::overflow_error("Mod overflow");
  }
  lli_t result = lhs % rhs;
  if (result < 0)
  {
    if (rhs > 0)
    {
      result += rhs;
    }
    else
    {
      result -= rhs;
    }
  }
  return result;
}

burukov::lli_t burukov::bitwiseAnd(const lli_t lhs, const lli_t rhs)
{
  return lhs & rhs;
}

void burukov::getInfix(std::istream &in,
    Stack< Queue< std::string > > &infix)
{
  char sym = 0;
  std::string current;
  Queue< std::string > expression;
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
        expression.clear();
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

void burukov::convertToPostfix(const Queue< std::string > &,
    Queue< std::string > &)
{
}

std::string burukov::calculate(const Queue< std::string > &)
{
  return "0";
}
