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

  void getInfix(std::istream& in, Stack< Queue < std::string > >& data)
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

      if (c == ' ' || c == '\t')
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

  void infixToPostfix(Stack< Queue < std::string > >& data, Stack< Queue < std::string > >& res)
  {
    while (!data.empty())
    {
      Stack<std::string> operators;
      Queue<std::string> current = data.drop();
      Queue<std::string> output;

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
        else if (isOperator(token))
        {
          while (!operators.empty() && getPriority(operators.top()) >= getPriority(token))
          {
            output.push(operators.top());
            operators.pop();
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
      res.push(output);
    }
  }

  bool isNumber(const std::string& token)
  {
    if (token.empty())
    {
      return false;
    }
    if (token[0] == '-' && token.size() > 1)
    {
      return true;
    }
    return std::isdigit(token[0]);
  }

  ll strToNum(const std::string& str)
  {
    try
    {
      return std::stoll(str);
    }
    catch (const std::invalid_argument&)
    {
      throw std::runtime_error("Not a number");
    }
    catch (const std::out_of_range&)
    {
      throw std::overflow_error("Number overflow");
    }
  }
}
