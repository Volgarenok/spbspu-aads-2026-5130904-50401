#include <iostream>
#include <fstream>
#include <exception>
#include "utils.hpp"
#include "../common/Stack.hpp"

int main(int argc, char* argv[])
{
  if (argc > 2)
  {
    std::cerr << "err\n";
    return 1;
  }

  try
  {
    std::ifstream ifs;
    if (argc == 2)
    {
      ifs.open(argv[1]);
      if (!ifs.is_open())
      {
        std::cerr << "err\n";
        return 1;
      }
    }

    std::istream& in = (argc == 2) ? ifs : std::cin;
    petrov::Stack<long long int> results = petrov::calcStream(in);

    while (!results.empty())
    {
      std::cout << results.drop();
      if (!results.empty())
      {
        std::cout << " ";
      }
    }
    if (argc == 1 || (argc == 2 && ifs.is_open()))
    {
       std::cout << "\n";
    }
  }
  catch (...)
  {
    std::cerr << "err\n";
    return 1;
  }

  return 0;
}
