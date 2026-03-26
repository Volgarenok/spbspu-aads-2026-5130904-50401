#include <iostream>
#include <string>
#include <fstream>
#include "functions.hpp"
#include "stack.hpp"

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
    if (file.peek() == EOF)
    {
    }
    else
    {
      input = &file;
    }
  }

std::string line;
zhuravleva::Stack < zhuravleva::value_t > results;
try
{
  while(std::getline(*input, line))
  {
    if (line.empty())
    {
      continue;
    }
    auto postfix = zhuravleva::infToPostfix(line);
    zhuravleva::value_t result = zhuravleva::calcPostfix(postfix);
    results.push(result);
  }
  if (!results.empty())
  {
    bool first = true;
    while (!results.empty())
    {
      if (!first)
      {
        std::cout << " ";
      }
        std::cout << results.drop();
      first = false;
    }
  }
    std::cout << "\n";
  } catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  return 0;
}
