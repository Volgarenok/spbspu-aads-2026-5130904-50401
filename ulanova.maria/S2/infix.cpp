#include "infix.hpp"
#include "stack.hpp"

#include <sstream>
#include <string>

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
  Stack< char > ops;
  Queue<std::string> output;

  std::stringstream s(expr);
  std::string token;

  while (s >> token)
  {
  if (!isOperator(token[0]) && token != "(" && token != ")")
  {
      output.push(token);
  }
  else if (token == "(")
  {
      ops.push('(');
  }
  else if (token == ")")
  {
      while (!ops.empty() && ops.front() != '(')
      {
      output.push(std::string(1, ops.front()));
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
      char op = token[0];
      while (!ops.empty() && priority(ops.front()) >= priority(op))
      {
        output.push(std::string(1, ops.front()));
        ops.pop();
      }
      ops.push(op);
  }
  }
  while (!ops.empty())
  {
    if (ops.front() == '(')
    {
      throw std::runtime_error("incorrect expression");
    }

    output.push(std::string(1, ops.front()));
    ops.pop();
  }
  return output;
}
