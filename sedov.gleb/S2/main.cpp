#include <iostream>
#include <fstream>
#include "queue.hpp"
#include "math_func.hpp"
#include "expression.hpp"

int main(int argc, char ** argv)
{
  using namespace sedov;
  Stack< Expression > infix;
  std::istream * inputStream = nullptr;
  std::ifstream file;

  if (argc < 2)
  {
    inputStream = &std::cin;
  }
  else if (argc == 2)
  {
    file.open(argv[1]);
    if (!file)
    {
      std::cerr << "Bad input file\n";
      return 1;
    }
    inputStream = &file;
  }
  else
  {
    std::cerr << "Usage: " << argv[0] << " [filename]\n";
    return 1;
  }

  try
  {
    getInfix(*inputStream, infix);
  }
  catch (...)
  {
    std::cerr << "Input errors\n";
    return 1;
  }

  if (infix.empty())
  {
    std::cout << "\n";
    return 0;
  }
  List< std::string > out;
  while (!infix.empty())
  {
    Expression inf = infix.top();
    infix.pop();
    Expression postfix = convertInfToPost(inf);
    std::string res = calculate(postfix);
    out.pushBack(res);
  }
  LIter< std::string > it = out.begin();
  std::cout << *it;
  ++it;
  for (; it != out.end(); ++it)
  {
    std::cout << " " << *it;
  }
  std::cout << "\n";
  return 0;
}
