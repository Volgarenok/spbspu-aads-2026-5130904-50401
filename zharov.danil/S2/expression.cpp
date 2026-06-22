#include "expression.hpp"
#include <stdexcept>
#include "math_functions.hpp"

namespace
{
  bool isOperator(const std::string& str)
  {
    std::string signs[] = {"+", "-", "/", "*", "%", "<<", "(", ")"};
    for (size_t i = 0; i < 8; ++i)
    {
      if (str == signs[i])
      {
        return true;
      }
    }
    return false;
  }

  int getPriority(const std::string& op)
  {
    if (op == "<<")
    {
      return 3;
    }
    if (op == "*" || op == "/")
    {
      return 1;
    }
    return 2;
  }
}

zharov::Queue< std::string > zharov::detail::getQueue(const std::string& line)
{
  Queue< std::string > res;
  std::string curr;
  for (size_t i = 0; line[i] != '\0'; ++i)
  {
    if (line[i] == ' ')
    {
      res.push(curr);
      curr.clear();
    }
    else
    {
      curr.push_back(line[i]);
    }
  }
  if (!curr.empty())
  {
    res.push(curr);
  }
  return res;
}

zharov::Queue< std::string > zharov::detail::getPostfix(Queue< std::string >& infix)
{
  Stack< std::string > stack;
  Queue< std::string > res;
  while (!infix.empty())
  {
    std::string curr = infix.front();
    infix.pop();
    if (!isOperator(curr))
    {
      res.push(curr);
    }
    else if (curr == "(")
    {
      stack.push(curr);
    }
    else if (curr == ")")
    {
      while (!stack.empty() && stack.top() != "(")
      {
        res.push(stack.top());
        stack.pop();
      }
      if (!stack.empty())
      {
        stack.pop();
      }
      else
      {
        throw std::logic_error("Bad sign (");
      }
    }
    else
    {
      while (!stack.empty() && stack.top() != "(")
      {
        if (getPriority(curr) >= getPriority(stack.top()))
        {
          res.push(stack.top());
          stack.pop();
        }
        else
        {
          break;
        }
      }
      stack.push(curr);
    }
  }
  while (!stack.empty())
  {
    if (stack.top() != "(")
    {
      res.push(stack.top());
      stack.pop();
    }
    else
    {
      throw std::logic_error("Bad sign (");
    }
  }
  return res;
}

long long zharov::detail::calculate(Queue< std::string >& postfix)
{
  Stack< ll_t > temp;
  while (!postfix.empty())
  {
    std::string curr = postfix.front();
    postfix.pop();
    if (isOperator(curr))
    {
      if (temp.size() < 2)
      {
        throw std::logic_error("Not enough operands for operator: " + curr);
      }
      else
      {
        ll_t b = temp.top();
        temp.pop();
        ll_t a = temp.top();
        temp.pop();
        if (curr == "+")
        {
          temp.push(add(a, b));
        }
        else if (curr == "-")
        {
          temp.push(sub(a, b));
        }
        else if (curr == "*")
        {
          temp.push(mul(a, b));
        }
        else if (curr == "/")
        {
          temp.push(div(a, b));
        }
        else if (curr == "%")
        {
          temp.push(mod(a, b));
        }
        else if (curr == "<<")
        {
          temp.push(bitShiftLeft(a, b));
        }
        else
        {
          throw std::logic_error("Unknown operator" + curr);
        }
      }
    }
    else
    {
      temp.push(std::stoll(curr));
    }
  }
  if (temp.size() != 1)
  {
    throw std::logic_error("Not enough operators");
  }
  ll_t result = temp.top();
  temp.pop();
  return result;
}

zharov::Expression::Expression(const std::string& line)
{
  Queue< std::string > infix = detail::getQueue(line);
  postfix_ = detail::getPostfix(infix);
}

long long zharov::Expression::calculate()
{
  Queue< std::string > postfix = postfix_;
  return detail::calculate(postfix);
}
