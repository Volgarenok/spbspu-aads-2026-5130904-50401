#include <iostream>
#include <fstream>
#include "queue.hpp"
#include "math_func.hpp"

int main(int argc, char ** argv)
{
  using namespace sedov;
  Stack< Queue< std::string > > infix;
  if (argc < 2)
  {
    try
    {
      getInfix(std::cin, infix);
    }
    catch (...)
    {
      std::cerr << "Input errors\n";
      return 1;
    }
  }
  else if (argc == 2)
  {
    std::ifstream input(argv[1]);
    if (!input)
    {
      std::cerr << "Bad input file\n";
      return 1;
    }
    try
    {
      getInfix(input, infix);
    }
    catch (...)
    {
      std::cerr << "Bad reading file\n";
      return 1;
    }
    input.close();
  }
  if (infix.empty())
  {
    std::cout << "\n";
    return 0;
  }
  List< std::string > out;
  while (!infix.empty())
  {
    Queue< std::string > inf = infix.top();
    infix.pop();
    Queue< std::string > postfix;
    convertInfToPost(inf, postfix);
    std::string res;
    try
    {
      res = calculate(postfix);
    }
    catch (const std::exception & e)
    {
      std::cerr << e.what() << "\n";
      return 1;
    }
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
}
