#include <iostream>
#include <string>
#include "mathoperations.hpp"
#include "myqueue.hpp"
#include "mystack.hpp"

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
  while(!a.empty())
  {
    if (isdigit(a.front()[0]))
    {
      postfix.push(a.front());
    }
    else if (a.front() == "(")
    {
      op_and_br.push(a.front());
    }
    else if (a.front() == ")")
    {
      while(op_and_br.front() != "(")
      {
        postfix.push(op_and_br.front());
        op_and_br.drop();
      }
      op_and_br.drop();
    }
    else
    {
      if (op_and_br.empty() || priority(a.front()) >= priority(op_and_br.front()))
      {
        op_and_br.push(a.front());
      }
      else
      {
        while (priority(a.front()) >= priority(op_and_br.front()) && !op_and_br.empty())
        {
          postfix.push(op_and_br.front());
          op_and_br.drop();
        }
      }
    }
    a.drop();
  }
  while(!op_and_br.empty())
  {
    postfix.push(op_and_br.front());
    op_and_br.drop();
  }
  return postfix;
}

goltsov::Queue< std::string > getLine(std::istream& input)
{
  goltsov::Queue< std::string > inf;
  std::string a;
  while (input >> a)
  {
    inf.push(a);\
    if (input.peek() == '\n' || input.peek() == EOF)
    {
      break;
    }
  }
  return inf;
}

int main()
{
  
}