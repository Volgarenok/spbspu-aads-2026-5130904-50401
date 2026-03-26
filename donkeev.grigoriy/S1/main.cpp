#include <iostream>
#include "list.hpp"
#include "functions.hpp"

int main()
{
  using namespace donkeev;
  List< std::pair< std::string, List< size_t > > > data{};
  read(data, std::cin);
  printInfo(data, std::cout);
}
