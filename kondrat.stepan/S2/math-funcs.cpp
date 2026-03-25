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
        while (!stackForTemp.empty() && isOperator(stackForTemp.first()) && getPriority(stackForTemp.first()) >= getPriority(token))
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
}