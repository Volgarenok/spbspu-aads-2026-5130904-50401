#include <iostream>
#include <string>
#include <fstream>
#include "functions.hpp"
#include "stack.hpp"

namespace
{
  bool isEmptyLine(const std::string& line)
  {
    for (size_t i = 0; i < line.size(); ++i)
    {
      if (line[i] != ' ')
      {
        return false;
      }
    }
    return true;
  }

  void printResults(zhuravleva::Stack< zhuravleva::value_t >& results)
  {
    if (results.empty())
    {
      std::cout << "\n";
      return;
    }
    std::cout << results.drop();
    while (!results.empty())
    {
      std::cout << " " << results.drop();
    }
    std::cout << "\n";
  }
}

int main(int argc, char* argv[])
{
  if (argc > 2)
  {
    std::cerr << "wrong parameters\n";
    return 1;
  }
  std::istream* input = &std::cin;
  std::ifstream file;
  if (argc == 2)
  {
    file.open(argv[1]);
    if (!file.is_open())
    {
      std::cerr << "cannot open file\n";
      return 1;
    }
    input = &file;
  }

  std::string line;
  zhuravleva::Stack< zhuravleva::value_t > results;
  while (std::getline(*input, line))
  {
    if (isEmptyLine(line))
    {
      continue;
    }
    try
    {
      zhuravleva::Queue< std::string > postfix = zhuravleva::infToPostfix(line);
      zhuravleva::value_t result = zhuravleva::calcPostfix(postfix);
      results.push(result);
    }
    catch (const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      return 1;
    }
  }
  printResults(results);
  return 0;
}
