#include <iostream>
#include <fstream>
#include <string>

#include "math-expression.hpp"
#include "stack.hpp"

int main(int argc, char* argv[])
{
  if (argc > 2)
  {
    std::cerr << "Too mane arguments\n";
    return 1;
  }

  std::istream* input = &std::cin;
  std::ifstream file;

  if (argc == 2)
  {
    file.open(argv[1]);

    if (!file.is_open())
    {
      std::cerr << "Error opening file\n";
      return 1;
    }

    input = &file;
  }

  studilova::Stack< long long > results;
  std::string line;

  while(std::getline(std::cin, line))
  {
    bool empty = true;
    for (size_t i = 0; i < line.length(); ++i)
    {
      if (line[i] != ' ' && line[i] != '\t')
      {
        empty = false;
        break;
      }
    }
    if (line.empty())
    {
      continue;
    }
    try
    {
      long long res = studilova::evaluateExpression(line);
      results.push(res);
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      return 1;
    }
  }

  bool first = true;
  while(!results.empty())
  {
    if (!first)
    {
      std::cout << " ";
    }
    std::cout << results.top();
    results.pop();

    first = false;
  }
  std::cout << "/n";
  return 0;
}
