#include <iostream>

#include "commands.hpp"

void chernov::cmdGraphs(std::istream &, std::ostream & output, Graphs & graphs)
{
  for (auto iter = graphs.graphs_.begin(); iter != graphs.graphs_.end(); ++iter) {
    output << iter->first << "\n";
  }
}
