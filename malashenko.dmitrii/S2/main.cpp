#include <iostream>
#include <fstream>
#include "queue.hpp"
#include "math_functions.hpp"
int main(int argc, char ** argv)
{
  using namespace malashenko;
  Stack< Queue< std::string > > infixAllData;

  if (argc < 2)
  {
    try
    {
      getInfixData(std::cin, infixAllData);
    } catch (...)
    {
      std::cerr << "Input problem\n";
      return 1;
    }
  } else if (argc == 2)
  {
    std::ifstream input(argv[1]);
     if (!input)
     {
      std::cerr << "Problem with input file opening\n";
      return 1;
    }
    try
    {
      getInfixData(input, infixAllData);
    } catch (...)
    {
      std::cerr << "Problem with file reading\n";
      return 1;
    }
    input.close();
  }
  List< std::string > out;

  while (!infixAllData.empty())
  {
    Queue< std::string > infixData = infixAllData.top();
    infixAllData.pop();
    Queue< std::string > postfixData;
    converInfixToPostfix(infixData, postfixData);
    std::string res;
    try
    {
      res = calculate(postfixData);
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      return 1;
    }

    out.push_back(res);
  }

  LIter< std::string > it = out.begin();
  std::cout << *it;
  ++it;
  for (; it != out.end(); ++it)
  {
    std::cout << ' ' << *it;
  }
  std::cout << '\n';
}
