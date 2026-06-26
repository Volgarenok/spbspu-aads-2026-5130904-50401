#ifndef HELPEDFUNC_HPP
#define HELPEDFUNC_HPP

#include <string>
#include <iostream>
#include <limits>
#include <fstream>

#include "robinTable.hpp"
#include "robinTableStructs.hpp"

namespace donkeev
{

  std::string nextWord(const std::string& str, size_t& pos);
  int inputInt(const std::string&);
  std::string inputString(const std::string&);
  size_t inputSize_t(const std::string&);
}

#endif
