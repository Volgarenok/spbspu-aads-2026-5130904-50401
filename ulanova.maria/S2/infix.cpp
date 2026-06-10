#include "infix.hpp"

#include <stdexcept>
#include <string>

#include "stack.hpp"

namespace
{
  bool isOperator(char c)
  {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
  }
  int priority(char op)
  {
    if (op == '+' || op == '-' || op == '^')
    {
      return 1;
    }
    if (op == '*' || op == '/' || op == '%')
    {
      return 2;
    }
    return 0;
  }
}
ulanova::Queue< std::string > ulanova::infix_to_postfix(const std::string& expr)
{
  Stack< std::string > ops;
  Queue<std::string> output;

  size_t pos = 0;
  while (pos < expr.size())
  {
    while (pos < expr.size() && expr[pos] == ' ')
    {
      ++pos;
    }

    if (pos == expr.size())
    {
      break;
    }

    size_t end = pos;
    while (end < expr.size() && expr[end] != ' ')
    {
      ++end;
    }

    std::string token = expr.substr(pos, end - pos);

    if (!isOperator(token[0]) && token != "(" && token != ")")
    {
      output.push(token);
    }
    else if (token == "(")
    {
      ops.push(token);
    }
    else if (token == ")")
    {
      while (!ops.empty() && ops.front() != "(")
      {
        output.push(ops.front());
        ops.pop();
      }
      if (ops.empty())
      {
        throw std::runtime_error("incorrect expression");
      }
      ops.pop();
    }
    else
    {
      std::string op = token;
      while (!ops.empty() && ops.front() != "(" && priority(ops.front()[0]) >= priority(op[0]))
      {
        output.push(ops.front());
        ops.pop();
      }
      ops.push(op);
    }

    pos = end;
  }

  while (!ops.empty())
  {
    if (ops.front() == "(")
    {
      throw std::runtime_error("incorrect expression");
    }
    output.push(ops.front());
    ops.pop();
  }

  return output;
}
