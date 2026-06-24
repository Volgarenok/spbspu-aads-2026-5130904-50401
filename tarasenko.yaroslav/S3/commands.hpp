#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <string>
#include "graph.hpp"
#include "hash_table.hpp"

namespace tarasenko
{
  using GraphTable = HashTable< std::string, Graph >;
  using Command = void (*)(std::istream&, std::ostream&, GraphTable&);

  void showGraphs(std::istream&, std::ostream& out, GraphTable& graphs);
  void showVertexes(std::istream& in, std::ostream& out, GraphTable& graphs);
  void showOutbound(std::istream& in, std::ostream& out, GraphTable& graphs);
  void showInbound(std::istream& in, std::ostream& out, GraphTable& graphs);
  void bind(std::istream& in, std::ostream&, GraphTable& graphs);
  void cut(std::istream& in, std::ostream&, GraphTable& graphs);
  void createGraph(std::istream& in, std::ostream&, GraphTable& graphs);
  void mergeGraphs(std::istream& in, std::ostream&, GraphTable& graphs);
  void extractGraph(std::istream& in, std::ostream&, GraphTable& graphs);
}

#endif
