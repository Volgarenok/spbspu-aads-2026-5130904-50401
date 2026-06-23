#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include "math-funcs.hpp"
#include "stack.hpp"

int main(int argc, char ** argv)
{
  if (argc > 2)
  {
    std::cerr << "Error\n";
    return 1;
  }

  std::ifstream file;
  std::istream * input = std::addressof(std::cin);

  if (argc == 2)
  {
    file.open(argv[1]);
    if (!file)
    {
      std::cerr << "bad file\n";
      return 1;
    }
    input = std::addressof(file);
  }

  kondrat::Stack< kondrat::ll > result;

  try
  {
    std::string line;
    while (std::getline(*input, line))
    {
      if (!line.empty())
      {
        result.push(kondrat::calculateExpression(line));
      }
    }

    if (!result.empty())
    {
      std::cout << result.front();
      result.pop();
    }

    while (!result.empty())
    {
      std::cout << ' ' << result.front();
      result.pop();
    }
    std::cout << '\n';
  }
  catch (const std::exception & e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
