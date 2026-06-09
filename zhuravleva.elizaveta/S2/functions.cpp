#include "functions.hpp"
#include <stdexcept>
#include <string>
#include "stack.hpp"

bool zhuravleva::isOperator(const std::string& op)
{
  return op == "+" || op == "-" || op == "*" ||
      op == "/" || op == "%" || op == "!";
}

int zhuravleva::priority(const std::string& op)
{
  if (op == "!")
  {
    return 3;
  }
  else if (op == "*" || op == "/" || op == "%")
  {
    return 2;
  }
  else if (op == "+" || op == "-")
  {
    return 1;
  }
  return 0;
}

zhuravleva::Queue< std::string > zhuravleva::infToPostfix(const std::string& line)
{
  Queue< std::string > output;
  Stack< std::string > operations;
  std::string part;

  for (size_t i = 0; i <= line.size(); ++i)
  {
    if (i == line.size() || line[i] == ' ')
    {
      if (part.empty())
      {
        continue;
      }
      if (part == "(")
      {
        operations.push(part);
      }
      else if (part == ")")
      {
        while (!operations.empty() && operations.top() != "(")
        {
          output.push(operations.top());
          operations.pop();
        }
        if (operations.empty())
        {
          throw std::runtime_error("brackets error");
        }
        operations.pop();
        if (!operations.empty() && operations.top() == "!")
        {
          output.push(operations.top());
          operations.pop();
        }
      }
      else if (isOperator(part))
      {
        while (!operations.empty() && isOperator(operations.top()) &&
            priority(operations.top()) >= priority(part))
        {
          output.push(operations.top());
          operations.pop();
        }
        operations.push(part);
      }
      else
      {
        output.push(part);
      }
      part.clear();
    }
    else
    {
      part += line[i];
    }
  }
  while (!operations.empty())
  {
    if (operations.top() == "(")
    {
      throw std::runtime_error("brackets error");
    }
    output.push(operations.top());
    operations.pop();
  }
  return output;
}

zhuravleva::value_t zhuravleva::applyBinary(const std::string& op, value_t a, value_t b)
{
  if (op == "+")
  {
    return add(a, b);
  }
  else if (op == "-")
  {
    return sub(a, b);
  }
  else if (op == "*")
  {
    return mul(a, b);
  }
  else if (op == "/")
  {
    return div(a, b);
  }
  else if (op == "%")
  {
    return mod(a, b);
  }

  throw std::runtime_error("unknown operator");
}

zhuravleva::value_t zhuravleva::calcPostfix(Queue< std::string > line)
{
  Stack< value_t > operands;
  while (!line.empty())
  {
    std::string part = line.front();
    line.pop();
    if (isOperator(part))
    {
      if (part == "!")
      {
        if (operands.empty())
        {
          throw std::runtime_error("not enough operands");
        }

        value_t value = operands.top();
        operands.pop();
        operands.push(bitNot(value));
      }
      else
      {
        if (operands.size() < 2)
        {
          throw std::runtime_error("not enough operands");
        }
        value_t second = operands.top();
        operands.pop();
        value_t first = operands.top();
        operands.pop();
        operands.push(applyBinary(part, first, second));
      }
    }
    else
    {
      operands.push(std::stoll(part));
    }
  }

  if (operands.size() != 1)
  {
    throw std::runtime_error("incorrect expression");
  }
  value_t result = operands.top();
  operands.pop();
  return result;
}
