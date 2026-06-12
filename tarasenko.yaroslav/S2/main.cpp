#include <fstream>
#include <iostream>
#include <string>
#include "functions.hpp"
#include "stack.hpp"

int main(int argc, char** argv)
{
  if (argc > 2)
  {
    std::cerr << "to many arguments\n";
    return 1;
  }
  try
  {
    std::ifstream file;
    std::istream* input = &std::cin;
    tarasenko::Stack< long long > results;
    std::string line;
    if (argc == 2)
    {
      file.open(argv[1]);
      if (!file.is_open())
      {
        std::cerr << "could not open file\n";
        return 1;
      }
      input = &file;
    }
    while (std::getline(*input, line))
    {
      if (!line.empty())
      {
        results.push(tarasenko::calculate(line));
      }
    }
    if (!results.empty())
    {
      std::cout << results.top();
      results.pop();
    }
    while (!results.empty())
    {
      std::cout << " " << results.top();
      results.pop();
    }
    std::cout << "\n";
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
