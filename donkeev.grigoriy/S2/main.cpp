#include <iostream>
#include <stdexcept>
#include <fstream>
#include "math-operations.hpp"
#include "queue.hpp"
#include "stack.hpp"

int main(int argc, char** argv)
{
  donkeev::Queue< donkeev::Queue< char > > infixExpression;
  if (argc > 2)
  {
    throw std::invalid_argument("Too many arguments");
  }
  else if (argc == 2)
  {
    std::ifstream input(argv[1]);
    if (!input)
    {
      std::cerr << "Can't open the file\n";
      return 1;
    }
    try
    {
      donkeev::readExpression(infixExpression, input);
    }
    catch (const std::runtime_error& e)
    {
      std::cerr << e.what() << '\n';
      return 1;
    }
  }
  else
  {
    donkeev::readExpression(infixExpression, std::cin);
  }

  donkeev::Stack< donkeev::llint_t > result;

  try
  {
    donkeev::calculate(result, infixExpression);
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  if (!result.empty())
  {
    std::cout << result.top();
    result.pop();
  }
  while (!result.empty())
  {
    std::cout << " " << result.top();
    result.pop();
  }
  std::cout << '\n';
}
