#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <utility>
#include <iostream>
#include "hash_table.hpp"
#include "vector.hpp"

namespace tarasenko
{
  struct EdgeInfo
  {
    std::string vertex;
    Vector< unsigned int > weights;
  };
  bool operator==(const EdgeInfo& lhs, const EdgeInfo& rhs);

  class Graph
  {
  public:
    void addVertex(const std::string& name);
    bool hasVertex(const std::string& name) const;

    void bind(const std::string& from, const std::string& to, unsigned int weight);
    void cut(const std::string& from, const std::string& to, unsigned int weight);

    Vector< std::string > getVertexes() const;
    Vector< EdgeInfo > getOutbound(const std::string& vertex) const;
    Vector< EdgeInfo > getInbound(const std::string& vertex) const;

    Graph extract(const Vector< std::string >& vertexes) const;

    void swap(Graph& rhs) noexcept;

  private:
    friend Graph merge(const Graph& lhs, const Graph& rhs);

    HashTable<
      std::pair< std::string, std::string >,
      Vector< unsigned int >
    > edges_;

    HashTable< std::string, bool > vertexes_;
  };

  Graph merge(const Graph& lhs, const Graph& rhs);
  HashTable< std::string, Graph > getFromFile(std::istream& input);
}

#endif
