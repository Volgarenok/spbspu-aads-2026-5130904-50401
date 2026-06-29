#ifndef GRAPHS_COMMANDS_HPP
#define GRAPHS_COMMANDS_HPP

#include <iostream>
#include "Hash-table.hpp"
#include "Graph.hpp"
#include "hash-structs.hpp"
#include "../common/top-it-vector.hpp"
#include "../common/vector-iterators.hpp"

namespace donkeev
{
  using GraphTable_t = donkeev::HashTable< std::string, donkeev::Graph, donkeev::GraphNameHash, donkeev::GraphEqual>;

  std::string nextWord(const std::string&, size_t&);

  void sortNames(topit::Vector< std::pair< std::string, donkeev::List< size_t > > >&);

  void sortNames(topit::Vector< std::string >&);

  void sortNumbers(topit::Vector< size_t >&);

  void printGrapsNames(GraphTable_t&, const std::string&, std::ostream&);

  void printVertexesNames(GraphTable_t&, const std::string&, std::ostream&);

  void printOutboundVertexesNames(GraphTable_t&, const std::string&, std::ostream&);

  void printInboundVertexesNames(GraphTable_t&, const std::string&, std::ostream&);

  void createEdge(GraphTable_t&, const std::string&, std::ostream&);

  void deleteEdge(GraphTable_t&, const std::string&, std::ostream&);

  void createGraph(GraphTable_t&, const std::string&, std::ostream&);

  void mergeGraphs(GraphTable_t&, const std::string&, std::ostream&);

  void extractGraph(GraphTable_t&, const std::string&, std::ostream&);
}
#endif
