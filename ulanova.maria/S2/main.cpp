#include "infix.hpp"
#include "postfix.hpp"
#include <iostream>
#include <string>
#include <fstream>

int main(int argc, char* argv[])
{
  std::istream* input = &std::cin;
  std::ifstream file;
  if (argc > 1)
  {
    file.open(argv[1]);
    if (!file.is_open())
    {
      std::cerr << "cannot open file\n";
      return 1;
    }
    input = &file;
  }
  ulanova::Stack<long long> results;
  std::string line;
  try
  {
    while (std::getline(*input, line))
    {
      if (line.empty())
      {
        continue;
      }
      std::string postfix = ulanova::infix_to_postfix(line);
      long long result = ulanova::postfix(postfix);
      results.push(result);
    }
    bool first = true;
    while (!results.empty())
    {
      if (!first)
      {
        std::cout << " ";
      }
      std::cout << results.drop() << "\n";
      first = false;
    }
  } catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  return 0;
}
