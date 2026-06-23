#include "math-funcs.hpp"
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <string>
#include "queue.hpp"
#include "stack.hpp"

namespace
{
  size_t getOperatorType(const std::string & token)
  {
    if (token == "#")
    {
      return 2;
    }
    if (token == "+" || token == "-" || token == "*" || token == "/" || token == "%")
    {
      return 1;
    }
    return 0;
  }

  bool isOperator(const std::string & token)
  {
    return getOperatorType(token) != 0;
  }

  size_t getPriority(const std::string & token)
  {
    if (token == "#")
    {
      return 3;
    }
    if (token == "*" || token == "/" || token == "%")
    {
      return 2;
    }
    if (token == "+" || token == "-")
    {
      return 1;
    }
    return 0;
  }

  kondrat::Queue< std::string > tokenize(const std::string & str)
  {
    kondrat::Queue< std::string > res;
    std::string curToken = "";
    for (size_t i = 0; i < str.size(); ++i)
    {
      if (str[i] != ' ')
      {
        curToken.push_back(str[i]);
      }
      else if (!curToken.empty())
      {
        res.push(curToken);
        curToken = "";
      }
    }
    if (!curToken.empty())
    {
      res.push(curToken);
    }
    return res;
  }

  bool shouldPopOperator(const kondrat::Stack< std::string > & stack, const std::string & token)
  {
    return !stack.empty()
        && isOperator(stack.front())
        && getPriority(stack.front()) >= getPriority(token);
  }

  kondrat::Queue< std::string > infixToPostfix(kondrat::Queue< std::string > & before)
  {
    kondrat::Queue< std::string > postfix;
    kondrat::Stack< std::string > stackForTemp;

    while (!before.empty())
    {
      std::string token = before.front();
      before.pop();
      if (token == "(")
      {
        stackForTemp.push(token);
      }
      else if (token == ")")
      {
        while (!stackForTemp.empty() && (stackForTemp.front() != "("))
        {
          postfix.push(stackForTemp.front());
          stackForTemp.pop();
        }

        if (stackForTemp.empty())
        {
          throw std::logic_error("invalid brackets");
        }

        stackForTemp.pop();
      }
      else if (isOperator(token))
      {
        while (shouldPopOperator(stackForTemp, token))
        {
          postfix.push(stackForTemp.front());
          stackForTemp.pop();
        }
        stackForTemp.push(token);
      }
      else
      {
        postfix.push(token);
      }
    }

    while (!stackForTemp.empty())
    {
      if (stackForTemp.front() == "(" || stackForTemp.front() == ")")
      {
        throw std::logic_error("invalid brackets");
      }
      postfix.push(stackForTemp.front());
      stackForTemp.pop();
    }
    return postfix;
  }

  kondrat::ll evaluatePostfix(kondrat::Queue< std::string > & postfix)
  {
    kondrat::Stack< kondrat::ll > values;

    while (!postfix.empty())
    {
      std::string token = postfix.front();
      postfix.pop();
      size_t type = getOperatorType(token);

      if (type == 0)
      {
        values.push(std::stoll(token));
      }
      else if (type == 1)
      {
        if (values.size() < 2)
        {
          throw std::logic_error("invalid expression");
        }

        kondrat::ll rhs = values.front();
        values.pop();
        kondrat::ll lhs = values.front();
        values.pop();

        values.push(kondrat::applyBinaryOperation(lhs, rhs, token));
      }
      else if (type == 2)
      {
        if (values.empty())
        {
          throw std::logic_error("invalid expression");
        }

        kondrat::ll value = values.front();
        values.pop();
        values.push(kondrat::applyUnaryOperation(value, token));
      }
    }

    if (values.size() != 1)
    {
      throw std::logic_error("invalid expression");
    }
    kondrat::ll result = values.front();
    values.pop();
    return result;
  }
}

kondrat::ll kondrat::add(ll lhs, ll rhs)
{
  if ((rhs > 0 && lhs > std::numeric_limits< ll >::max() - rhs)
      || (rhs < 0 && lhs < std::numeric_limits< ll >::min() - rhs))
  {
    throw std::overflow_error("overflow");
  }
  return lhs + rhs;
}

kondrat::ll kondrat::sub(ll lhs, ll rhs)
{
  if ((rhs < 0 && lhs > std::numeric_limits< ll >::max() + rhs)
      || (rhs > 0 && lhs < std::numeric_limits< ll >::min() + rhs))
  {
    throw std::overflow_error("overflow");
  }
  return lhs - rhs;
}

kondrat::ll kondrat::mul(ll lhs, ll rhs)
{
  if (lhs == 0 || rhs == 0)
  {
    return 0;
  }

  if (lhs == -1 && rhs == std::numeric_limits< ll >::min())
  {
    throw std::overflow_error("overflow");
  }
  if (rhs == -1 && lhs == std::numeric_limits< ll >::min())
  {
    throw std::overflow_error("overflow");
  }

  if (lhs > 0)
  {
    if (rhs > 0)
    {
      if (lhs > std::numeric_limits< ll >::max() / rhs)
      {
        throw std::overflow_error("overflow");
      }
    }
    else
    {
      if (rhs < std::numeric_limits< ll >::min() / lhs)
      {
        throw std::overflow_error("overflow");
      }
    }
  }
  else
  {
    if (rhs > 0)
    {
      if (lhs < std::numeric_limits< ll >::min() / rhs)
      {
        throw std::overflow_error("overflow");
      }
    }
    else
    {
      if (lhs != 0 && rhs < std::numeric_limits< ll >::max() / lhs)
      {
        throw std::overflow_error("overflow");
      }
    }
  }
  return lhs * rhs;
}

kondrat::ll kondrat::divide(ll lhs, ll rhs)
{
  if (rhs == 0)
  {
    throw std::logic_error("division by zero");
  }
  if (lhs == std::numeric_limits< ll >::min() && rhs == -1)
  {
    throw std::overflow_error("overflow");
  }
  return lhs / rhs;
}

kondrat::ll kondrat::mod(ll lhs, ll rhs)
{
  if (rhs == 0)
  {
    throw std::logic_error("division by zero");
  }

  ll result = lhs % rhs;
  ll modBase = rhs < 0 ? -rhs : rhs;

  if (result < 0)
  {
    result += modBase;
  }
  return result;
}

kondrat::ll kondrat::reverseNumber(ll value)
{
  ll result = 0;

  while (value != 0)
  {
    ll digit = value % 10;
    value /= 10;

    if (result > std::numeric_limits< ll >::max() / 10
        || result < std::numeric_limits< ll >::min() / 10)
    {
      throw std::overflow_error("overflow");
    }

    result *= 10;

    if ((digit > 0 && result > std::numeric_limits< ll >::max() - digit)
        || (digit < 0 && result < std::numeric_limits< ll >::min() - digit))
    {
      throw std::overflow_error("overflow");
    }
    result += digit;
  }
  return result;
}

kondrat::ll kondrat::applyBinaryOperation(ll lhs, ll rhs, const std::string & token)
{
  if (token == "+")
  {
    return add(lhs, rhs);
  }
  if (token == "-")
  {
    return sub(lhs, rhs);
  }
  if (token == "*")
  {
    return mul(lhs, rhs);
  }
  if (token == "/")
  {
    return divide(lhs, rhs);
  }
  if (token == "%")
  {
    return mod(lhs, rhs);
  }
  throw std::logic_error("unknown operator");
}

kondrat::ll kondrat::applyUnaryOperation(ll value, const std::string & token)
{
  if (token == "#")
  {
    return reverseNumber(value);
  }
  throw std::logic_error("unknown operator");
}

kondrat::ll kondrat::calculateExpression(const std::string & str)
{
  Queue< std::string > tokens = tokenize(str);
  tokens = infixToPostfix(tokens);
  return evaluatePostfix(tokens);
}
