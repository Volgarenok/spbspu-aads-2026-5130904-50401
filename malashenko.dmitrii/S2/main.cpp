#include <iostream>
#include "queue.hpp"
#include "math_functions.hpp"
int main()
{
  using namespace malashenko;
  
  Queue< std::string > infixData;
  getInfixData(std::cin, infixData);
  std::cout << infixData.front() << ' ' << infixData.back() << '\n';
}
