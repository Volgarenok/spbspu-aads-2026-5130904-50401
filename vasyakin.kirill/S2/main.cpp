#include <iostream>
#include <stdexcept>
#include <fstream>
#include <string>
#include <cstddef>
#include "stack.hpp"
#include "math_op.hpp"

int main(int argc, char** argv)
{
  if (argc > 2)
  {
    std::cerr << "Too much" << '\n';
    return 1;
  }

  std::istream* input = &std::cin;
  std::ifstream file;

  if (argc == 2)
  {
    file.open(argv[1]);
    if (!file.is_open())
    {
      std::cerr << "Error: Cannot open file " << argv[1] << '\n';
      return 1;
    }
    input = &file;
  }
  vasyakin::Stack< long long > results;
  std::string line;

  try
  {
    while (std::getline(*input, line))
    {
      if (line.empty())
      {
        continue;
      }

      long long res = vasyakin::evaluateExpression(line);
      results.push(res);
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }

  if (!results.empty())
  {
    std::cout << results.top();
    results.pop();

    while (!results.empty())
    {
      std::cout << " " << results.top();
      results.pop();
    }
  }
  std::cout << '\n';

  return 0;
}
