#include <iostream>
#include <string>
#include <fstream>
#include "examplesolution.hpp"
#include "mathoperations.hpp"
#include "myqueue.hpp"
#include "mystack.hpp"

namespace goltsov
{
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

  goltsov::Stack< long long int > result;
  while(* input)
  {
    if (input->peek() == '\n')
    {
      input->get();
      continue;
    }
    try
    {
      auto infix = goltsov::getLine(*input);
      if (infix.empty())
      {
        continue;
      }
      if (infix.empty() && input->eof())
      {
        break;
      }
      result.push(goltsov::eval(goltsov::converToPostfix(infix)));
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      return 1;
    }
  }
  while (result.size() > 1)
  {
    std::cout << result.front() << ' ';
    result.drop();
  }
  std::cout << result.front() << '\n';
  result.drop();
}
