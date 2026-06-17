#ifndef INPUT_HPP
#define INPUT_HPP

#include <cstddef>
#include <istream>
#include <string>
#include "../common/vector/vector.hpp"
#include "graph.hpp"

namespace lukashevich {
  bool parseUnsigned(const std::string & text, size_t & value);
  Vector< std::string > splitLine(const std::string & line);
  void readGraphs(std::istream & in, GraphTable & graphs);
}

#endif
