#include <iostream>
#include <fstream>
#include "calculator.hpp"
int main(int argc, char ** argv)
{
  using namespace malashenko;
  Calculator calc;
  std::istream* in = &std::cin;
  std::ifstream input;
  if (argc == 2)
  {
    input.open(argv[1]);
    if (!input)
    {
      std::cerr << "Problem with input file opening\n";
      return 1;
    }
    in = &input;
  }

  try
  {
    calc.getInfixData(*in);
  }
  catch (...)
  {
    std::cerr << "Input problem\n";
    return 1;
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
