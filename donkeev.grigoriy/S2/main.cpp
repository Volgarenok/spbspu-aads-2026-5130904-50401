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

  donkeev::calculate(result, infixExpression);

  donkeev::printResult(result, std::cout);
}
