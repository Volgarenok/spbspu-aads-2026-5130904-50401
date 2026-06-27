#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <utility>

#include <list.hpp>

#include "hash_table.hpp"
#include "siphash.hpp"

namespace burukov
{
  class Graph
  {
  public:
    using EdgeKey = std::pair< std::string, std::string >;
    using WeightList = List< size_t >;
    using VertexList = List< std::string >;
    using EdgeTable = HashTable< EdgeKey, WeightList, PairHash, std::equal_to< EdgeKey > >;

    Graph() = default;
    explicit Graph(size_t bucketHint);

    void addVertex(const std::string &vertexName);
    void addEdge(const std::string &from, const std::string &to, size_t weight);
    void removeEdge(const std::string &from, const std::string &to, size_t weight);

    bool hasVertex(const std::string &vertexName) const;

    const VertexList &vertices() const noexcept;
    const EdgeTable &edges() const noexcept;

  private:
    VertexList vertices_;
    EdgeTable edges_;
  };
}

#endif
