#include <iostream>
#include "utility.hpp"
#include <list.hpp>

int main()
{
  hvostov::List< std::pair< std::string, hvostov::List< size_t > > > list;
  try {
    list = hvostov::getData(std::cin);
  } catch (...) {
    std::cerr << "Bad input!\n";
    hvostov::printInfo(std::cout, list);
    return 1;
  }
  hvostov::printInfo(std::cout, list);
  try {
    hvostov::printResult(std::cout, list);
    std::cout << "\n";
  } catch (const std::overflow_error& e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
