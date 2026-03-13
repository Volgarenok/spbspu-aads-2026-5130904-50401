#include <iostream>
#include "io.hpp"

using namespace levkin;
int main()
{
  Data data;
  readData(std::cin, data);
  printNames(std::cout, data);
  printTransposed(std::cout, data);
}
