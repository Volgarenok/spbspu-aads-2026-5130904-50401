#ifndef HELPEDFUNC_HPP
#define HELPEDFUNC_HPP

#include <string>
#include <iostream>
#include <limits>
#include <fstream>

#include "Ad.hpp"
#include "robinTable.hpp"
#include "robinTableStructs.hpp"

namespace donkeev
{
  using AdTable = donkeev::RobinTable<size_t, Ad, SizeTHash, SizeTEqual>;

  std::string nextWord(const std::string&, size_t&);
  int inputInt(const std::string&);
  std::string inputString(const std::string&);
  size_t inputSize_t(const std::string&);

  void saveCarToDataBase(const Car&, const std::string&);
  size_t getHistroySize();
  size_t generateNextAdId(AdTable&, size_t);
}

#endif
