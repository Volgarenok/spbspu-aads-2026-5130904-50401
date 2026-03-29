#include <iostream>
#include "list.hpp"
#include "utility.hpp"
#include <limits>
int main()
{
  hvostov::List< std::pair< std::string, hvostov::List< size_t > > > list;
  try {
    list = hvostov::getData(std::cin);
  } catch (const std::overflow_error& e) {
    std::cerr << e.what() << "\n";
    hvostov::printInfo(std::cout, list);
    return 1;
  }
  if (list.empty()) {
    std::cout << "0\n";
    return 0;
  }
  hvostov::printInfo(std::cout, list);
  try {
    hvostov::printResult(std::cout, list);
  } catch (const std::overflow_error& e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
