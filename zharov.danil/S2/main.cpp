#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include "expression.hpp"

int main(int argc, char** argv)
{
  zharov::Stack< long long > results;
  try
  {
    std::ifstream file;
    std::istream* in = std::addressof(std::cin);
    if (argc > 1)
    {
      file.open(argv[1]);
      in = std::addressof(file);
    }
    std::string line;
    while (!std::getline(*in, line).eof())
    {
      if (line.empty())
      {
        continue;
      }
      results.push(zharov::Expression(line).calculate());
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
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
