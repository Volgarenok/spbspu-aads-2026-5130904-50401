#include <iostream>
#include <fstream>
#include "stack.hpp"
#include "functions.hpp"

int main(int argc, char** argv)
{
  if (argc > 1)
  {
    std::cerr << "to many arguments\n";
    return 1;
  }
  if (argc)
  {
    std::ifstream file(argv[0]);
    if (!file.is_open())
    {
      std::cerr << "could not open file\n";
      return 1;
    }
    tarasenko::Stack< long long > results;
    std::string line;
    while (std::getline(file, line))
    {
      results.push(tarasenko::calculate(line));
    }
  }
}
