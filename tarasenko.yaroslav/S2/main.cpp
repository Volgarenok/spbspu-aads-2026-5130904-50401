#include <iostream>
#include <fstream>
#include "stack.hpp"
#include "functions.hpp"

int main(int argc, char** argv)
{
  if (argc > 2)
  {
    std::cerr << "to many arguments\n";
    return 1;
  }
  try
  {
    if (argc == 2)
    {
      std::ifstream file(argv[1]);
      if (!file.is_open())
      {
        std::cerr << "could not open file\n";
        return 1;
      }
      tarasenko::readStreamAndPrintResults(file);
    }
    else
    {
      tarasenko::readStreamAndPrintResults(std::cin);
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
