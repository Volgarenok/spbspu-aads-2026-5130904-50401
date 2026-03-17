#include <iostream>
#include <string>
#include <fstream>
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
      if (prev != " " && !isdigit(prev[0]))
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
    res = res * 10 + a[i];
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
    std::string op1 = postfix.front();
    postfix.drop();
    if (!isdigit(op1[0]))
    {
      a = result.front();
      result.drop();
      b = result.front();
      result.drop();
      operation = op1;
    }
    else
    {
      std::string op2 = postfix.front();
      postfix.drop();
      if (!isdigit(op2[0]))
      {
        a = convertStringToLLI(op2);
        b = result.front();
        result.drop();
        operation = op2;
      }
      else
      {
        std::string op3 = postfix.front();
        postfix.drop();
        a = convertStringToLLI(op1);
        b = convertStringToLLI(op2);
        operation = op3;
      }
    }
    if (operation == "+")
    {
      result.push(goltsov::addition(a, b));
    }
    else if (operation == "-")
    {
      result.push(goltsov::subtraction(a, b));
    }
    else if (operation == "*")
    {
      result.push(goltsov::multiplication(a, b));
    }
    else if (operation == "/")
    {
      result.push(goltsov::division(a, b));
    }
    else if (operation == "%")
    {
      result.push(goltsov::remOfDiv(a, b));
    }
    else if (operation == "##")
    {
      result.push(goltsov::concatenation(a, b));
    }
  }
  return result.front();
}

goltsov::Queue< std::string > getLine(std::istream& input)
{
  goltsov::Queue< std::string > inf;
  std::string a;
  while (input && input.peek() != '\n' && input.peek() != EOF)
  {
    if (input.peek() == ' ' || input.peek() == '\t')
    {
      input.get();
    }
    else
    {
      input >> a;
      inf.push(a);
    }
  }

  if (input.peek() == '\n')
  {
    input.get();
  }

  return inf;
}

int main(int argc, char** argv)
{
  std::ifstream input_file;
  std::istream* input = & std::cin;
  if (argc > 1)
  {
    input_file.open(argv[1]);
    input = & input_file;
  }
  std::cout << eval(converToPostfix(getLine(* input)));
}
