#include <fstream>
#include <iostream>
#include "mathFunctions.hpp"

int main(int argc, char **argv)
{
  burukov::Stack< burukov::Queue< std::string > > infix;
  if (argc < 2)
  {
    try
    {
      burukov::getInfix(std::cin, infix);
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
      burukov::getInfix(input, infix);
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

  burukov::Stack< std::string > resultStack;
  while (!infix.empty())
  {
    const burukov::Queue< std::string > inf = infix.top();
    infix.pop();
    burukov::Queue< std::string > postfix;
    try
    {
      burukov::convertToPostfix(inf, postfix);
      const std::string res = burukov::calculate(postfix);
      resultStack.push(res);
    }
    catch (const std::exception &exc)
    {
      std::cerr << exc.what() << "\n";
      return 1;
    }
  }

  burukov::Queue< std::string > results;
  while (!resultStack.empty())
  {
    results.push(resultStack.top());
    resultStack.pop();
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
