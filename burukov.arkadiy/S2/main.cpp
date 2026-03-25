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

  std::cout << "Expressions: " << infix.size() << "\n";
  return 0;
}
