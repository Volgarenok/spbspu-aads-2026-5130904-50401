#include <iostream>
#include "queue.hpp"
#include "math_functions.hpp"
int main()
{
  using namespace malashenko;
  
  Queue< std::string > infixData;
  getInfixData(std::cin, infixData);
  Queue< std::string > postfixData;
  converInfixToPostfix(infixData, postfixData);
  postfixData.show();  
}
