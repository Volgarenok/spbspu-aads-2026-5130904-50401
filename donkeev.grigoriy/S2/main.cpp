#include <iostream>
#include <stdexcept>
#include "math-operations.hpp"
#include "queue.hpp"
#include "stack.hpp"

using llint_t = long long int;

int main(int argc, char** argv)
{
  donkeev::Queue< donkeev::Queue< char > > infixExpression;
  if (argc > 2)
  {
    throw std::invalid_argument("Too many arguments");
  }
  else if (argc == 2)
  {
    try{
      donkeev::readFromFile(infixExpression, argv[1]);
    }
    catch (const std::runtime_error& e)
    {
      std::cerr << e.what() << '\n';
      return 1;
    }
  }
  else
  {
    donkeev::readFromTerminal(infixExpression, std::cin);
  }

  donkeev::Stack< llint_t > result;

  try
  {
    donkeev::calculate(result, infixExpression);
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  donkeev::printResult(result, std::cout);
}
