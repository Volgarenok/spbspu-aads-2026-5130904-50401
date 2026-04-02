#include <iostream>
#include "list.hpp"
#include "functions.hpp"

int main()
{
  using namespace donkeev;
  List< std::pair< std::string, List< size_t > > > data{};
  read(data, std::cin);
  try
  {
    printInfo(data, std::cout);
  }
  catch (const std::overflow_error& e)
  {
    std::cout << e.what() << "\n";
    return 1;
  }
}
