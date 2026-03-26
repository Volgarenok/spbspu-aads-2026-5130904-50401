#include <iostream>
#include <string>
#include <fstream>
#include "functions.hpp"
#include "stack.hpp"

int main(int argc, char* argv[])
{
   std::ifstream file;
   std::istream* input = &std::cin;
   if (argc > 1) {
    file.open(argv[1]);
    if (!file.is_open()) {
      std::cerr << "Cannot open file: " << argv[1] << "\n";
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
  bool hasInput = false;
  try
  {
    while(std::getline(*input, line))
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
      hasInput = true;
      auto postfix = zhuravleva::infToPostfix(line);
      zhuravleva::value_t result = zhuravleva::calcPostfix(postfix);
      results.push(result);
    }
    if (!hasInput) {
      std::cout << std::endl;
      return 0;
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
    if (!std::cin.eof())
    {
      std::cerr << "input error";
      return 1;
    }
    std::cout << std::endl;
  }catch(const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  return 0;
}
