#include "examplesolution.hpp"
#include <iostream>
#include <string>
#include "myqueue.hpp"
#include "mystack.hpp"
#include "mathoperations.hpp"

long long int goltsov::detail::priority(std::string a)
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
goltsov::Queue< std::string > goltsov::converToPostfix(goltsov::Queue< std::string > a)
{
  goltsov::Queue< std::string > postfix;
  goltsov::Stack< std::string > op_and_br;
  std::string prev = " ";
  while (!a.empty())
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
      while (op_and_br.top() != "(" && !op_and_br.empty())
      {
        postfix.push(op_and_br.top());
        op_and_br.pop();
      }
      if (op_and_br.empty())
      {
        throw std::logic_error("Bad input expression");
      }
      op_and_br.pop();
    }
    else
    {
      if (prev != " " && !isdigit(prev[0]) && prev != ")")
      {
        throw std::logic_error("Bad input expression");
      }
      if (op_and_br.empty() || detail::priority(a.front()) > detail::priority(op_and_br.top()))
      {
        op_and_br.push(a.front());
      }
      else
      {
        while (!op_and_br.empty()
          && (detail::priority(a.front()) <= detail::priority(op_and_br.top())) && op_and_br.top() != "(")
        {
          postfix.push(op_and_br.top());
          op_and_br.pop();
        }
        op_and_br.push(a.front());
      }
    }
    prev = a.front();
    a.pop();
  }
  while (!op_and_br.empty())
  {
    postfix.push(op_and_br.top());
    op_and_br.pop();
  }
  return postfix;
}
long long int goltsov::eval(goltsov::Queue< std::string > postfix)
{
  goltsov::Stack< long long int > result;
  while (!postfix.empty())
  {
    try
    {
      result.push(std::stoll(postfix.front()));
      postfix.pop();
    }
    catch (...)
    {
      long long int a, b;
      std::string operation;
      operation = postfix.front();
      postfix.pop();
      a = result.top();
      result.pop();
      b = result.top();
      result.pop();
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
  }
  if (result.size() != 1)
  {
    throw;
  }
  else
  {
    return result.top();
  }
}
