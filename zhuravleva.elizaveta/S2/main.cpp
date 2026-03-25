#include <iostream>
#include <string>
#include "functions.hpp"
#include "stack.hpp"

int main()
{
  std::string line;
  zhuravleva::Stack < zhuravleva::value_t > results;
  try
  {
    while(std::getline(std::cin, line))
    {
      bool empt = true;
      for (char c : line)
      {
        if (c != ' ')
        {
          empt = false;
          break;
        }
      }
      if (empt)
      {
        continue;
      }
      auto postfix = zhuravleva::infToPostfix(line);
      zhuravleva::value_t result = zhuravleva::calcPostfix(postfix);

      results.push(result);
    }
    bool first = true;
    while(!results.empty())
    {
      if(!first)
      {
        std::cout << " ";
      }
      std::cout << results.drop();
      first = false;
    }
    std::cout << " ";
    if (!std::cin.eof())
    {
      std::cerr << "input error";
      return 1;
    }
  }catch(const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  return 0;
}
