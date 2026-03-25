#include <iostream>
#include <fstream>
#include "mathFunctions.hpp"

int main(int argc, char **argv)
{
  using namespace burukov;
  Stack< Queue< std::string > > infix;
  if (argc < 2)
  {
    try
    {
      getInfix(std::cin, infix);
    }
    catch (...)
    {
      std::cerr << "Input error\n";
      return 1;
    }
  }
  else if (argc == 2)
  {
    std::ifstream input(argv[1]);
    if (!input)
    {
      std::cerr << "Cannot open file\n";
      return 1;
    }
    try
    {
      getInfix(input, infix);
    }
    catch (...)
    {
      std::cerr << "Read error\n";
      return 1;
    }
  }

  if (infix.empty())
  {
    std::cout << "\n";
    return 0;
  }

  Queue< std::string > results;
  while (!infix.empty())
  {
    const Queue< std::string > inf = infix.top();
    infix.pop();
    Queue< std::string > postfix;
    convertToPostfix(inf, postfix);
    try
    {
      const std::string res = calculate(postfix);
      results.push(res);
    }
    catch (const std::exception &exc)
    {
      std::cerr << exc.what() << "\n";
      return 1;
    }
  }

  std::cout << results.front();
  results.pop();
  while (!results.empty())
  {
    std::cout << " " << results.front();
    results.pop();
  }
  std::cout << "\n";

  return 0;
}
