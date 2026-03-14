#include <iostream>
#include "list.hpp"
#include "utility.hpp"

int main()
{
  hvostov::List< std::pair< std::string, hvostov::List< size_t > > > list = hvostov::getData(std::cin);
  hvostov::Liter< std::pair< std::string, hvostov::List< size_t > > > it = list.begin();
  hvostov::printResult(std::cout, list);
  return 0;
}
