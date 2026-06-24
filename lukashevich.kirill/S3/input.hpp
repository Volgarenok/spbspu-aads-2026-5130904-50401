#ifndef INPUT_HPP
#define INPUT_HPP

#include <iosfwd>

#include "graph.hpp"

namespace lukashevich {
  bool hasExtraData(std::istream & in);
  void readGraphs(std::istream & in, GraphTable & graphs);
}

#endif
