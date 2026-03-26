#include <iostream>
#include <string>

#include "math-expression.hpp"
#include "stack.hpp"

int main()
{
  studilova::Stack< long long > results;
  std::string line;

  while(std::getline(std::cin, line))
  {
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
