#include "math-funcs.hpp"
#include <iostream>

int main(int argc, char ** argv)
{
  using namespace kondrat;

  if (argc > 2)
  {
    std::cerr << "Error\n";
    return 1;
  }

  Stack< ll > result;

  try
  {
    if (argc == 2)
    {
      std::ifstream file(argv[1]);
      if (!file)
      {
        std::cerr << "bad file\n";
        return 1;
      }
      readExpressions(file, result);
    }
    else
    {
      readExpressions(std::cin, result);
    }
    printResults(result, std::cout);
  }
  catch(const std::exception & e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
