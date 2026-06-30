#include <iostream>
#include <string>
#include <fstream>
#include "examplesolution.hpp"
#include "mathoperations.hpp"
#include "myqueue.hpp"
#include "mystack.hpp"

namespace
{
  goltsov::Queue< std::string > getLine(std::istream& input)
  {
    goltsov::Queue< std::string > inf;
    std::string line;
    if (!std::getline(input, line))
    {
      return inf;
    }
    std::string a;
    for (size_t i = 0; i < line.size(); ++i)
    {
      if (line[i] == ' ' || line[i] == '\t')
      {
        if (!a.empty())
        {
          inf.push(a);
          a.clear();
        }
      }
      else
      {
        a += line[i];
      }
    }
    if (!a.empty())
    {
      inf.push(a);
    }
    return inf;
  }
}

int main(int argc, char** argv)
{
  std::ifstream input_file;
  std::istream* input = &std::cin;
  if (argc > 1)
  {
    input_file.open(argv[1]);
    input = &input_file;
  }
  goltsov::Stack< long long int > result;
  while (*input)
  {
    try
    {
      goltsov::Queue< std::string > infix = getLine(*input);
      if (infix.empty() && input->eof())
      {
        break;
      }
      if (infix.empty())
      {
        continue;
      }
      result.push(goltsov::eval(goltsov::converToPostfix(infix)));
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      return 1;
    }
  }
  if (!result.empty())
  {
    while (result.size() > 1)
    {
      std::cout << result.top() << ' ';
      result.pop();
    }
    std::cout << result.top() << '\n';
    result.pop();
  }
  else
  {
    std::cout << '\n';
  }
}
