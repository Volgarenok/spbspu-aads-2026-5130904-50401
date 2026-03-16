#include <iostream>
#include "mathoperations.hpp"
#include "myqueue.hpp"
#include "mystack.hpp"

long long int priority(char a)
{
  if (a == '-' || a == '+')
  {
    return 1;
  }
  else if (a == '%')
  {
    return 2;
  }
  else if (a == '*' || a == '/')
  {
    return 3;
  }
  else if (a == '#')
  {
    return 4;
  }
  else
  {
    return -1;
  }
}

goltsov::Queue< char > converToPostfix(goltsov::Queue< char > a)
{
  goltsov::Queue< char > postfix;
  goltsov::Stack< char > op_and_br;
  while(!a.empty())
  {
    if (isdigit(a.front()))
    {
      postfix.push(a.front());
    }
    else if (a.front() == '(')
    {
      op_and_br.push(a.front());
    }
    else if (a.front() == ')')
    {
      while(op_and_br.front() != '(')
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

int main()
{
  goltsov::Queue< char > a;
  a.push('(');
  a.push('5');
  a.push('+');
  a.push('3');
  a.push(')');
  a.push('-');
  a.push('2');
  a.push('*');
  a.push('3');
  goltsov::Queue< char > b = converToPostfix(a);
  while (!b.empty())
  {
    std::cout << b.front() << ' ';
    b.drop();
  }
  std::cout << '\n';
}