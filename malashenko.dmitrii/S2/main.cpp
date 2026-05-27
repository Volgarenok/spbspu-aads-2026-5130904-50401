#include <iostream>
#include <fstream>
#include "calculator.hpp"
int main(int argc, char ** argv)
{
  using namespace malashenko;
  Calculator calc;
  if (argc < 2)
  {
    try
    {
      calc.getInfixData(std::cin);
    }
    catch (...)
    {
      std::cerr << "Input problem\n";
      return 1;
    }
  }
  else if (argc == 2)
  {
    std::ifstream input(argv[1]);
    if (!input)
    {
      std::cerr << "Problem with input file opening\n";
      return 1;
    }
    try
    {
      calc.getInfixData(input);
    }
    catch (...)
    {
      std::cerr << "Problem with file reading\n";
      return 1;
    }
  }

  if (calc.isInfixAllDataEmpty())
  {
    std::cout << '\n';
    return 0;
  }

  while (!calc.isInfixAllDataEmpty())
  {
    calc.convertInfixToPostfix();
    try
    {
      calc.calculateFromPostfix();
    }
    catch (const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      return 1;
    }

    calc.pushToOut();
  }

  calc.printOutList(std::cout);
  std::cout << '\n';
}
