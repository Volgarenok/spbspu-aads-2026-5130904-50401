#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "Hash-table.hpp"
#include "hash-structs.hpp"
#include "../common/list.hpp"

namespace donkeev
{
  using edgesPair_t = std::pair< std::string, std::string >;
  using EdgeTable_t = HashTable< edgesPair_t, List< size_t >, VertexPairHash, VertexPairEqual >;
  struct Graph
  {
    EdgeTable_t table_;
    topit::Vector< std::string > uniqueVertexes_;

    Graph();
    Graph(size_t, size_t);
    Graph(const Graph&);
    Graph(Graph&&) noexcept;
    Graph& operator=(const Graph&);
    Graph& operator=(Graph&&) noexcept;

    void addEdge(const std::string, const std::string, const size_t);
    void addVertex(const std::string&);

    void deleteEdge(const std::string, const std::string, const size_t);
  };
}

#endif
