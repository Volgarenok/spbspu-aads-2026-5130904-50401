#include "infix.hpp"
#include "postfix.hpp"
#include <iostream>
#include <string>

int main()
{
  ulanova::Stack<long long> results;
  std::string line;

  try
  {
    while (std::getline(std::cin, line))
    {
      if (line.empty())
      {
        continue; //
      }
      std::string postfix = ulanova::infix_to_postfix(line);
      long long result = ulanova::postfix(postfix);
      results.push(result);
    }
    while (!results.empty())
    {
      std::cout << results.drop() << "\n";
    }
  } catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  return 0;
}
