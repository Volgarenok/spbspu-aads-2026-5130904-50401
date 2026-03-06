#include <iostream>
#include <string>
#include "mylist.hpp"
#include "functions.hpp"

int main()
{
  size_t size = 0;
  goltsov::List< std::pair< std::string, goltsov::List< int > > > data;
  try
  {
    data = goltsov::getData(std::cin, size);
  }
  catch(const std::overflow_error& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  goltsov::printResult(std::cout, data, size);
}
