#include "math_func.hpp"

#include <cstdlib>
#include <istream>
#include <limits>
#include <stdexcept>

namespace
{
  const int bitOrPriority = 0;
  const int addSubPriority = 1;
  const int multDivModPriority = 2;

  bool isDelimiter(char symbol)
  {
    return symbol == ' ' || symbol == '\t' || symbol == '\r';
  }

  yalovsky::lli_t parseNumber(const std::string& token)
  {
    std::size_t parsedSize = 0;
    yalovsky::lli_t value = 0;
    try
    {
      value = std::stoll(token, &parsedSize);
    }
    catch (const std::exception&)
    {
      throw std::invalid_argument("Input error");
    }

    if (parsedSize != token.size())
    {
      throw std::invalid_argument("Input error");
    }
    return value;
  }
}

bool yalovsky::isOperator(const std::string& token)
{
  return token == "+" || token == "-" || token == "*" || token == "/" || token == "%"
      || token == "|";
}

int yalovsky::getPriority(const std::string& token)
{
  if (token == "|")
  {
    return bitOrPriority;
  }
  if (token == "+" || token == "-")
  {
    return addSubPriority;
  }
  if (token == "*" || token == "/" || token == "%")
  {
    return multDivModPriority;
  }
  throw std::invalid_argument("Input error");
}

yalovsky::lli_t yalovsky::add(const lli_t& lhs, const lli_t& rhs)
{
  const lli_t maxValue = std::numeric_limits< lli_t >::max();
  const lli_t minValue = std::numeric_limits< lli_t >::min();
  if ((rhs > 0 && lhs > maxValue - rhs) || (rhs < 0 && lhs < minValue - rhs))
  {
    throw std::overflow_error("Add overflow");
  }
  return lhs + rhs;
}

yalovsky::lli_t yalovsky::sub(const lli_t& lhs, const lli_t& rhs)
{
  const lli_t maxValue = std::numeric_limits< lli_t >::max();
  const lli_t minValue = std::numeric_limits< lli_t >::min();
  if ((rhs > 0 && lhs < minValue + rhs) || (rhs < 0 && lhs > maxValue + rhs))
  {
    throw std::overflow_error("Sub overflow");
  }
  return lhs - rhs;
}

yalovsky::lli_t yalovsky::mult(const lli_t& lhs, const lli_t& rhs)
{
  const lli_t maxValue = std::numeric_limits< lli_t >::max();
  const lli_t minValue = std::numeric_limits< lli_t >::min();

  if (lhs == 0 || rhs == 0)
  {
    return 0;
  }
  if (lhs == -1)
  {
    if (rhs == minValue)
    {
      throw std::overflow_error("Mult overflow");
    }
    return -rhs;
  }
  if (rhs == -1)
  {
    if (lhs == minValue)
    {
      throw std::overflow_error("Mult overflow");
    }
    return -lhs;
  }
  if (lhs > 0)
  {
    if ((rhs > 0 && lhs > maxValue / rhs) || (rhs < 0 && rhs < minValue / lhs))
    {
      throw std::overflow_error("Mult overflow");
    }
  }
  else
  {
    if ((rhs > 0 && lhs < minValue / rhs) || (rhs < 0 && lhs < maxValue / rhs))
    {
      throw std::overflow_error("Mult overflow");
    }
  }
  return lhs * rhs;
}

yalovsky::lli_t yalovsky::div(const lli_t& lhs, const lli_t& rhs)
{
  const lli_t minValue = std::numeric_limits< lli_t >::min();
  if (rhs == 0)
  {
    throw std::invalid_argument("Div by zero");
  }
  if (lhs == minValue && rhs == -1)
  {
    throw std::overflow_error("Div overflow");
  }
  return lhs / rhs;
}

yalovsky::lli_t yalovsky::mod(const lli_t& lhs, const lli_t& rhs)
{
  const lli_t minValue = std::numeric_limits< lli_t >::min();
  if (rhs == 0)
  {
    throw std::invalid_argument("Mod by zero");
  }
  if (lhs == minValue && rhs == -1)
  {
    throw std::overflow_error("Mod overflow");
  }

  lli_t result = lhs % rhs;
  if (result < 0)
  {
    result += std::llabs(rhs);
  }
  return result;
}

yalovsky::lli_t yalovsky::bitOr(const lli_t& lhs, const lli_t& rhs)
{
  return lhs | rhs;
}

void yalovsky::getInfix(std::istream& in, Stack< Queue< std::string > >& expressions)
{
  char symbol = 0;
  std::string token;
  Queue< std::string > expression;

  while (in.get(symbol))
  {
    if (symbol == '\n')
    {
      if (!token.empty())
      {
        expression.push(token);
        token.clear();
      }
      if (!expression.empty())
      {
        expressions.push(expression);
        expression.clear();
      }
    }
    else if (isDelimiter(symbol))
    {
      if (!token.empty())
      {
        expression.push(token);
        token.clear();
      }
    }
    else
    {
      token += symbol;
    }
  }

  if (!token.empty())
  {
    expression.push(token);
  }
  if (!expression.empty())
  {
    expressions.push(expression);
  }
}

void yalovsky::convertInfToPost(const Queue< std::string >& infix, Queue< std::string >& postfix)
{
  Queue< std::string > input(infix);
  Stack< std::string > operators;

  while (!input.empty())
  {
    const std::string token = input.drop();
    if (token == "(")
    {
      operators.push(token);
    }
    else if (token == ")")
    {
      bool wasOpenBracket = false;
      while (!operators.empty())
      {
        if (operators.top() == "(")
        {
          operators.pop();
          wasOpenBracket = true;
          break;
        }
        postfix.push(operators.drop());
      }
      if (!wasOpenBracket)
      {
        throw std::invalid_argument("Input error");
      }
    }
    else if (isOperator(token))
    {
      while (!operators.empty() && operators.top() != "("
          && getPriority(operators.top()) >= getPriority(token))
      {
        postfix.push(operators.drop());
      }
      operators.push(token);
    }
    else
    {
      postfix.push(token);
    }
  }

  while (!operators.empty())
  {
    if (operators.top() == "(")
    {
      throw std::invalid_argument("Input error");
    }
    postfix.push(operators.drop());
  }
}

std::string yalovsky::calculate(const Queue< std::string >& postfix)
{
  Queue< std::string > input(postfix);
  Stack< lli_t > values;

  while (!input.empty())
  {
    const std::string token = input.drop();
    if (!isOperator(token))
    {
      values.push(parseNumber(token));
      continue;
    }

    if (values.size() < 2)
    {
      throw std::invalid_argument("Input error");
    }

    const lli_t rhs = values.drop();
    const lli_t lhs = values.drop();
    lli_t result = 0;
    if (token == "+")
    {
      result = add(lhs, rhs);
    }
    else if (token == "-")
    {
      result = sub(lhs, rhs);
    }
    else if (token == "*")
    {
      result = mult(lhs, rhs);
    }
    else if (token == "/")
    {
      result = div(lhs, rhs);
    }
    else if (token == "%")
    {
      result = mod(lhs, rhs);
    }
    else
    {
      result = bitOr(lhs, rhs);
    }
    values.push(result);
  }

  if (values.size() != 1)
  {
    throw std::invalid_argument("Input error");
  }
  return std::to_string(values.top());
}
