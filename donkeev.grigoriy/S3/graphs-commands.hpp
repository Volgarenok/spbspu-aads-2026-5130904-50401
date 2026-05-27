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
  using graphsHashTable_t = donkeev::HashTable< std::string, donkeev::Graph, donkeev::GraphNameHash, donkeev::GraphEqual>;
  
  std::string nextWord(const std::string&, size_t&);

  void sortNames(topit::Vector< std::string >&);

  void sortNumbers(topit::Vector< size_t >&);

  void printGrapsNames(graphsHashTable_t, const std::string&, std::ostream&);
  
  void printVertexesNames(graphsHashTable_t, const std::string&, std::ostream&);

  void printOutboundVertexesNames(graphsHashTable_t, const std::string&, std::ostream&);
/*
  void printInboundVertexesNames(graphsHashTable_t, const std::string&, std::ostream&);

  void createEdge(graphsHashTable_t, const std::string&, std::ostream&);

  void deleteEdge(graphsHashTable_t, const std::string&, std::ostream&);

  void createGraph(graphsHashTable_t, const std::string&, std::ostream&);

  void mergeGraphs(graphsHashTable_t, const std::string&, std::ostream&);

  void extractGraph(graphsHashTable_t, const std::string&, std::ostream&);*/
}
#endif
