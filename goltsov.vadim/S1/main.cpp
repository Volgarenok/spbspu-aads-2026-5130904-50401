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
    data = goltsov::getData(std::cin, size);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  goltsov::printResult(std::cout, data, size);
}
