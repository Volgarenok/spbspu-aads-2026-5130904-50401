#include <iostream>
#include <string>
#include "examplesolution.hpp"
#include "myqueue.hpp"
#include "mystack.hpp"
#include "mathoperations.hpp"

namespace goltsov
{
  long long int priority(std::string a)
  {
    if (a == "-" || a == "+")
    {
      return 1;
    }
    else if (a == "%")
    {
      return 2;
    }
    else if (a == "*" || a == "/")
    {
      return 3;
    }
    else if (a == "##")
    {
      return 4;
    }
    else
    {
      return -1;
    }
  }

  goltsov::Queue< std::string > converToPostfix(goltsov::Queue< std::string > a)
  {
    goltsov::Queue< std::string > postfix;
    goltsov::Stack< std::string > op_and_br;
    std::string prev = " ";
    while(!a.empty())
    {
      if (isdigit(a.front()[0]))
      {
        if (prev != " " && isdigit(prev[0]))
        {
          throw std::logic_error("Bad input expression");
        }
        postfix.push(a.front());
      }
      else if (a.front() == "(")
      {
        if (prev != " " && (isdigit(prev[0]) || prev == ")"))
        {
          throw std::logic_error("Bad input expression");
        }
        op_and_br.push(a.front());
      }
      else if (a.front() == ")")
      {
        if ((prev != " " && !isdigit(prev[0])) || prev == "(")
        {
          throw std::logic_error("Bad input expression");
        }
        while(op_and_br.front() != "(" && !op_and_br.empty())
        {
          postfix.push(op_and_br.front());
          op_and_br.drop();
        }
        if (op_and_br.empty())
        {
          throw std::logic_error("Bad input expression");
        }
        op_and_br.drop();
      }
      else
      {
        if (prev != " " && !isdigit(prev[0]) && prev != ")")
        {
          throw std::logic_error("Bad input expression");
        }
        if (op_and_br.empty() || priority(a.front()) > priority(op_and_br.front()))
        {
          op_and_br.push(a.front());
        }
        else
        {
          while (!op_and_br.empty()
            && (priority(a.front()) <= priority(op_and_br.front())) && op_and_br.front() != "(")
          {
            postfix.push(op_and_br.front());
            op_and_br.drop();
          }
          op_and_br.push(a.front());
        }
      }
      prev = a.front();
      a.drop();
    }
    while(!op_and_br.empty())
    {
      postfix.push(op_and_br.front());
      op_and_br.drop();
    }

    return postfix;
  }

  long long int convertStringToLLI(std::string a)
  {
    long long int res = 0;
    for (size_t i = 0; i < a.size(); ++i)
    {
      res = res * 10 + (a[i] - '0');
    }
    return res;
  }

  long long int eval(goltsov::Queue< std::string > postfix)
  {
    goltsov::Stack< long long int > result;
    while (!postfix.empty())
    {
      long long int a, b;
      std::string operation;
      while (!postfix.empty() && isdigit(postfix.front()[0]))
      {
        result.push(convertStringToLLI(postfix.front()));
        postfix.drop();
      }
      try
      {
        operation = postfix.front();
        postfix.drop();
      }
      catch (...)
      {
        if (result.size() != 1)
        {
          throw;
        }
        else
        {
          return result.front();
        }
      }
      a = result.front();
      result.drop();
      b = result.front();
      result.drop();
      if (operation == "+")
      {
        result.push(goltsov::addition(b, a));
      }
      else if (operation == "-")
      {
        result.push(goltsov::subtraction(b, a));
      }
      else if (operation == "*")
      {
        result.push(goltsov::multiplication(b, a));
      }
      else if (operation == "/")
      {
        result.push(goltsov::division(b, a));
      }
      else if (operation == "%")
      {
        result.push(goltsov::remOfDiv(b, a));
      }
      else if (operation == "##")
      {
        result.push(goltsov::concatenation(b, a));
      }
    }
    return result.front();
  }
}
