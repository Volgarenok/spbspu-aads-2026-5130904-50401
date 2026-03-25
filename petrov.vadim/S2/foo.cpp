#include "foo.hpp"

namespace petrov
{
  bool isOperator(const std::string& token)
  {
    return token == "+" || token == "-" || token == "*" ||
           token == "/" || token == "%" || token == "#";
  }
  
  size_t getPriority(const std::string& op)
  {
    if (op == "#") return 3;
    if (op == "*" || op == "/" || op == "%") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
  }

  void getInfix(const std::istream& in, Stack< Queue < std::string > >& data)
  {
    char c;
    std::string token;
    Queue<std::string> current;
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

      if (c == ' ' | c == '\t')
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
  }
}
