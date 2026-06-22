#include <iostream>
#include "functions.hpp"

int main()
{
  petrov::List< std::pair< std::string, petrov::List< size_t > > > data;
  try
  {
    petrov::mkData(std::cin, data);
  }
  catch(const std::overflow_error& e)
  {
    std::cerr << e.what() << '\n';
    try
    {
      petrov::printAllList(data, std::cout);
      std::cout << '\n';
    }
    catch (const std::exception& e)
    {
      std::cerr << e.what() << '\n';
    }
    return 1;
  }

  try
  {
    petrov::printAllList(data, std::cout);
    std::cout << '\n';
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  return 0;
}
