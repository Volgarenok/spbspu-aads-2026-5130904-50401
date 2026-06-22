#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <fstream>
#include <functional>
#include <string>
#include "graph.hpp"

namespace zharov
{
  using graphs_table =
    HashTable< std::string, Graph, Blake2Hasher< std::string >, std::equal_to< std::string > >;
  void graphs(std::ostream&, std::istream&, const graphs_table&);
  void vertexes(std::ostream&, std::istream&, const graphs_table&);
  void outbound(std::ostream&, std::istream&, const graphs_table&);
  void inbound(std::ostream&, std::istream&, const graphs_table&);

  void bind(std::ostream&, std::istream&, graphs_table&);
  void cut(std::ostream&, std::istream&, graphs_table&);

  void create(std::ostream&, std::istream&, graphs_table&);
  void merge(std::ostream&, std::istream&, graphs_table&);
  void extract(std::ostream&, std::istream&, graphs_table&);
}

#endif
