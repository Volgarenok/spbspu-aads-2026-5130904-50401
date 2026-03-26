
#include <exception>
#include <fstream>
#include <iostream>

#include "ExpressionProcess.hpp"

int main(int argc, char **argv)
{
  if (argc > 2)
  {
    std::cerr << "too many comand line params, must be zero or one!\n";
    return 1;
  }
  khasnulin::Stack< long long > answers;
  try
  {
    if (argc == 2)
    {
      std::ifstream file(argv[1]);
      if (!file.is_open())
      {
        std::cerr << "can't open file " << argv[1] << "\n";
        return 1;
      }
      answers = khasnulin::readAndProcessAllExpressions(file);
    }
    else
    {
      answers = khasnulin::readAndProcessAllExpressions(std::cin);
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << "get error, while reading and processing expressions: " << e.what() << "\n";
    return 1;
  }
  catch (...)
  {
    std::cerr << "get unknown error\n";
    return 1;
  }

  while (!answers.empty())
  {
    std::cout << answers.top() << "\n";
    answers.pop();
  }

  return 0;
}
