#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <cstddef>
#include <string>
#include <utility>
#include <vector/top-it-vector.hpp>
#include "hash-func.hpp"
#include "hash-table.hpp"

namespace kondrat
{

  using WeightList = Vector< size_t >;
  using EdgeInfo = std::pair< std::string, WeightList >;

  struct Graph
  {
    void addVertex(const std::string & vertex);
    bool hasVertex(const std::string & vertex) const;
    void bind(const std::string & from, const std::string & to, size_t weight);
    void cut(const std::string & from, const std::string & to, size_t weight);
    Vector< std::string > getVertexes() const;
    Vector< EdgeInfo > getOutbound(const std::string & vertex) const;
    Vector< EdgeInfo > getInbound(const std::string & vertex) const;
    void mergeFrom(const Graph & first, const Graph & second);
    void extractFrom(const Graph & graph, const Vector< std::string > & vertexes);
    void swap(Graph & graph) noexcept;

  private:
    HashTable< std::string, bool, blake2 > vertexes_;
    HashTable< EdgeKey, WeightList, edgeBlake2 > edges_;
  };
}

#endif
