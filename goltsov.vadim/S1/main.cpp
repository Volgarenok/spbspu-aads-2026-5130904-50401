#include <iostream>
#include <string>
#include "mylist.hpp"
#include "functions.hpp"

int main()
{
  size_t size = 0;
  goltsov::List< std::pair< std::string, goltsov::List< size_t > > > data = goltsov::getData(std::cin, size);
  goltsov::printResult(std::cout, data, size);
}
