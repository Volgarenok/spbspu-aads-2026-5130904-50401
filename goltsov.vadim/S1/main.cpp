#include <iostream>
#include <string>
#include "mylist.hpp"
#include "functions.hpp"

int main()
{
  size_t size = 0;
  goltsov::List< std::pair< std::string, goltsov::List< size_t > > > data;
  try
  {
    goltsov::getData(data, std::cin, size);
  }
  catch(const std::overflow_error& e)
  {
    std::cerr << e.what() << '\n';
    try
    {
      goltsov::printResult(std::cout, data, size);
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      return 1;
    }
    return 1;
  }
  try
  {
    goltsov::printResult(std::cout, data, size);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
