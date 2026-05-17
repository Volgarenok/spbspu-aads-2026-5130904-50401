#include <iostream>

#include "commands.hpp"

void chernov::cmdGraphs(std::istream &, std::ostream & output, Graphs & graphs)
{
  graphs.showGraphs(output);
}

void chernov::cmdVertexes(std::istream & input, std::ostream & output, Graphs & graphs)
{
  std::string graph_name;
  input >> graph_name;
  graphs.showGraphVertexes(graph_name, output);
}
