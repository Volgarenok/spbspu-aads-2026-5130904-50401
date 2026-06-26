#include <string>
#include <iostream>
#include <limits>
#include <fstream>

#include "Car.hpp"

namespace donkeev
{
  std::string nextWord(const std::string&, size_t&);
  int inputInt(const std::string&);
  std::string inputString(const std::string&);

  void saveCarToDataBase(const Car&, const std::string&);
}
