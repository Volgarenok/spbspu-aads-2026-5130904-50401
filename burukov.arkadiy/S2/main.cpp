#include <iostream>
#include "mathFunctions.hpp"

int main()
{
  std::cout << burukov::isOperand("42") << "\n";
  std::cout << burukov::getPriority("+") << "\n";
  return 0;
}
