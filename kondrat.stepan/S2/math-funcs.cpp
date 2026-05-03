#include "math-funcs.hpp"

namespace kondrat
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

  Queue< std::string > tokenize(const std::string & str)
  {
    Queue< std::string > res;
    std::string cur_token = "";
    for (size_t i = 0; i < str.size(); ++i)
    {
      if (str[i] != ' ')
      {
        cur_token += std::string(1, str[i]);
      }
      else if (!cur_token.empty())
      {
        res.push(cur_token);
        cur_token = "";
      }
    }
    if (!cur_token.empty())
    {
      res.push(cur_token);
    }
    return res;
  }

  Queue< std::string > infixToPostfix(Queue< std::string > & before)
  {
    Queue< std::string > postfix;
    Stack< std::string > stackForTemp;

    while (!before.empty())
    {
      std::string token = before.drop();
      if (token == "(")
      {
        stackForTemp.push(token);
      }
      else if (token == ")")
      {
        while (!stackForTemp.empty() && (stackForTemp.first() != "("))
        {
          postfix.push(stackForTemp.drop());
        }

        if (stackForTemp.empty())
        {
          throw std::logic_error("invalid brackets");
        }

        stackForTemp.drop();
      }
      else if (isOperator(token))
      {
        while (!stackForTemp.empty()
              && isOperator(stackForTemp.first())
              && getPriority(stackForTemp.first()) >= getPriority(token))
        {
          postfix.push(stackForTemp.drop());
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
      if (stackForTemp.first() == "(" || stackForTemp.first() == ")")
      {
        throw std::logic_error("invalid brackets");
      }
      postfix.push(stackForTemp.drop());
    }
    return postfix;
  }

  ll add(ll lhs, ll rhs)
  {
    if ((rhs > 0 && lhs > std::numeric_limits< ll >::max() - rhs)
        || (rhs < 0 && lhs < std::numeric_limits< ll >::min() - rhs))
    {
      throw std::overflow_error("overflow");
    }
    return lhs + rhs;
  }

  ll sub(ll lhs, ll rhs)
  {
    if ((rhs < 0 && lhs > std::numeric_limits< ll >::max() + rhs)
        || (rhs > 0 && lhs < std::numeric_limits< ll >::min() + rhs))
    {
      throw std::overflow_error("overflow");
    }
    return lhs - rhs;
  }

  ll mul(ll lhs, ll rhs)
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

  ll divide(ll lhs, ll rhs)
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

  ll mod(ll lhs, ll rhs)
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

  ll reverseNumber(ll value)
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

  ll applyBinaryOperation(ll lhs, ll rhs, const std::string & token)
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

  ll applyUnaryOperation(ll value, const std::string & token)
  {
    if (token == "#")
    {
      return reverseNumber(value);
    }
    throw std::logic_error("unknown operator");
  }

  ll evaluatePostfix(Queue< std::string > & postfix)
  {
    Stack< ll > values;

    while (!postfix.empty())
    {
      std::string token = postfix.drop();
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

        ll rhs = values.drop();
        ll lhs = values.drop();

        values.push(applyBinaryOperation(lhs, rhs, token));
      }
      else if (type == 2)
      {
        if (values.empty())
        {
          throw std::logic_error("invalid expression");
        }

        ll value = values.drop();
        values.push(applyUnaryOperation(value, token));
      }
    }

    if (values.size() != 1)
    {
      throw std::logic_error("invalid expression");
    }
    return values.drop();
  }

  void readExpressions(std::istream & in, Stack< ll > & result)
  {
    std::string line;

    while (std::getline(in, line))
    {
      if (line.empty())
      {
        continue;
      }

      Queue< std::string > tokens = tokenize(line);
      tokens = infixToPostfix(tokens);
      result.push(evaluatePostfix(tokens));
    }
  }

  void printResults(Stack < ll > & result, std::ostream & out)
  {
    bool first = true;
    while (!result.empty())
    {
      if (!first)
      {
        out << ' ';
      }

      out << result.drop();
      first = false;
    }
    out << "\n";
  }
}
