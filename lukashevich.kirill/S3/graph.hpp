#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iosfwd>
#include <string>
#include <utility>
#include "../common/vector/vector.hpp"
#include "hash-table.hpp"
#include "hasher.hpp"

namespace lukashevich {
  using Weight = size_t;
  using EdgeKey = std::pair< std::string, std::string >;
  using WeightVector = Vector< Weight >;

  class Graph {
  public:
    using VertexTable = HashTable< std::string, bool, Blake2StringHash, StringEqual >;
    using EdgeTable = HashTable< EdgeKey, WeightVector, Blake2EdgeHash, EdgeEqual >;

    Graph();

    bool hasVertex(const std::string & vertex) const;
    void addVertex(const std::string & vertex);
    void bind(const std::string & from, const std::string & to, Weight weight);
    void cut(const std::string & from, const std::string & to, Weight weight);

    const VertexTable & getVertexes() const noexcept;
    const EdgeTable & getEdges() const noexcept;

    void printVertexes(std::ostream & out) const;
    void printOutbound(const std::string & vertex, std::ostream & out) const;
    void printInbound(const std::string & vertex, std::ostream & out) const;

  private:
    VertexTable vertexes_;
    EdgeTable edges_;
  };

  using GraphTable = HashTable< std::string, Graph, Blake2StringHash, StringEqual >;

  void addGraphCopy(GraphTable & graphs, const std::string & name, const Graph & graph);
}

#endif
