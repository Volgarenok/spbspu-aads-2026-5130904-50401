#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "Hash-table.hpp"
#include "hash-structs.hpp"
#include "../common/list.hpp"

namespace donkeev
{
  class Graph
  {
    size_t edgesCount_;
    donkeev::HashTable< std::pair< std::string, std::string >, donkeev::List< size_t >, donkeev::VertexPairHash, donkeev::VertexPairEqual > table_;

  public:
    Graph(size_t, size_t);
    void addEdge(const std::string, const std::string, const size_t);
  };
}

#endif
