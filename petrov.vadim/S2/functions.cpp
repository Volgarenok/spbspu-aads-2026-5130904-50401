#include "functions.hpp"

bool petrov::details::isOperator(const std::string& token)
{
  return token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == "#";
}

size_t petrov::details::getPriority(const std::string& op)
{
  if (op == "#")
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

petrov::Stack< petrov::Queue < std::string > > petrov::getInfix(std::istream& in)
{
  char c;
  std::string token;
  Queue< std::string > current;
  Stack< Queue < std::string > > data;
  while (in.get(c))
  {
    if (c == '\n')
    {
      if (!token.empty())
      {
        current.push(token);
        token.clear();
      }
      if (!current.empty())
      {
        data.push(current);
        current.clear();
      }
    }
    else if (c == ' ' || c == '\t')
    {
      if (!token.empty())
      {
        current.push(token);
        token.clear();
      }
    }
    else
    {
      token += c;
    }
  }
  if (!token.empty())
  {
    current.push(token);
  }
  if (!current.empty())
  {
    data.push(current);
  }
  return data;
}

petrov::Queue< std::string > petrov::infixToPostfix(Queue< std::string > current)
{
  Stack< std::string > operators;
  Queue< std::string > output;

  while (!current.empty())
  {
    std::string token = current.front();
    current.pop();

    if (token == "(")
    {
      operators.push(token);
    }
    else if (token == ")")
    {
      while (!operators.empty() && operators.top() != "(")
      {
        output.push(operators.top());
        operators.pop();
      }
      if (!operators.empty())
      {
        operators.pop();
      }
    }
    else if (details::isOperator(token))
    {
      while (!operators.empty() && operators.top() != "(")
      {
        size_t topPriority = details::getPriority(operators.top());
        size_t currPriority = details::getPriority(token);

        if ((token != "#" && topPriority >= currPriority)
            || (token == "#" && topPriority > currPriority))
        {
          output.push(operators.top());
          operators.pop();
        }
        else
        {
          break;
        }
      }
      operators.push(token);
    }
    else
    {
      output.push(token);
    }
  }

  while (!operators.empty())
  {
    output.push(operators.top());
    operators.pop();
  }
  return output;
}

petrov::ll petrov::pickOperation(ll a, ll b, const std::string& op)
{
  if (op == "+")
  {
    return plus(a, b);
  }
  if (op == "-")
  {
    return minus(a, b);
  }
  if (op == "*")
  {
    return mult(a, b);
  }
  if (op == "/")
  {
    return div(a, b);
  }
  if (op == "%")
  {
    return mod(a, b);
  }
  throw std::invalid_argument("Unknown operator");
}

petrov::ll petrov::calculatePostfix(Queue< std::string > postfix)
{
  Stack< ll > st;
  while (!postfix.empty())
  {
    std::string token = postfix.front();
    postfix.pop();

    if (token == "#")
    {
      if (st.empty())
      {
        throw std::logic_error("Not enough operands");
      }
      ll a = st.top();
      st.pop();
      st.push(details::reverseNumber(a));
    }
    else if (details::isOperator(token))
    {
      if (st.size() < 2)
      {
        throw std::logic_error("Not enough operands");
      }
      ll b = st.top();
      st.pop();
      ll a = st.top();
      st.pop();
      st.push(pickOperation(a, b, token));
    }
    else
    {
      st.push(std::stoll(token));
    }
  }

  if (st.size() != 1)
  {
    throw std::logic_error("Too many or no operands");
  }

  ll result = st.top();
  st.pop();
  return result;
}
