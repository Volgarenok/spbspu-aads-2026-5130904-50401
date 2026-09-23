#include <fstream>
#include <iostream>
#include <string>

#include "../common/list.hpp"
#include "math_func.hpp"

int main(int argc, char** argv)
{
  yalovsky::Stack< yalovsky::Queue< std::string > > infixExpressions;

  if (argc > 2)
  {
    std::cerr << "Wrong parameters\n";
    return 1;
  }

  if (argc == 2)
  {
    std::ifstream input(argv[1]);
    if (!input)
    {
      std::cerr << "Bad input file\n";
      return 1;
    }

    try
    {
      yalovsky::getInfix(input, infixExpressions);
    }
    catch (const std::exception& exception)
    {
      std::cerr << exception.what() << '\n';
      return 1;
    }
  }
  else
  {
    try
    {
      yalovsky::getInfix(std::cin, infixExpressions);
    }
    catch (const std::exception& exception)
    {
      std::cerr << exception.what() << '\n';
      return 1;
    }
  }

  yalovsky::List< std::string > results;
  try
  {
    while (!infixExpressions.empty())
    {
      yalovsky::Queue< std::string > postfix;
      yalovsky::convertInfToPost(infixExpressions.drop(), postfix);
      results.pushBack(yalovsky::calculate(postfix));
    }
  }
  catch (const std::exception& exception)
  {
    std::cerr << exception.what() << '\n';
    return 1;
  }

  if (!results.empty())
  {
    yalovsky::List< std::string >::const_iterator it = results.cbegin();
    std::cout << *it;
    ++it;
    for (; it != results.cend(); ++it)
    {
      std::cout << ' ' << *it;
    }
  }
  std::cout << '\n';
  return 0;
}
