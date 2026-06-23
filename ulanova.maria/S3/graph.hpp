#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>

#include "top_it_vector.hpp"
#include "hash_table.hpp"
#include "hash_functions.hpp"

namespace ulanova
{
  struct EdgeKey
  {
    std::string from;
    std::string to;
    unsigned weight;

    EdgeKey();
    EdgeKey(const std::string& edgeFrom, const std::string& edgeTo, unsigned edgeWeight);

    bool operator==(const EdgeKey& rhs) const;
  };

  template< class Hash, class Flavor >
  void hash_append(Hash& hash, const Flavor& flavor, const EdgeKey& key)
  {
    boost::hash2::hash_append(hash, flavor, key.from);
    boost::hash2::hash_append(hash, flavor, key.to);
    boost::hash2::hash_append(hash, flavor, key.weight);
  }

  using EdgeKeyHash = HmacHash< EdgeKey >;
  using EdgeKeyEqual = EqualTo< EdgeKey >;

  struct Edge
  {
    std::string vertex;
    unsigned weight;

    Edge();
    Edge(const std::string& vertexName, unsigned edgeWeight);

    bool operator==(const Edge& rhs) const;
  };

  class Graph
  {
  public:
    Graph();
    explicit Graph(const std::string& name);

    const std::string& getName() const noexcept;

    bool hasVertex(const std::string& vertex) const;
    void addVertex(const std::string& vertex);

    bool hasEdge(const std::string& from, const std::string& to, unsigned weight) const;
    void addEdge(const std::string& from, const std::string& to, unsigned weight);
    bool removeEdge(const std::string& from, const std::string& to, unsigned weight);

    Vector< std::string > getVertices() const;
    Vector< Edge > getOutbound(const std::string& vertex) const;
    Vector< Edge > getInbound(const std::string& vertex) const;

    Graph merge(const std::string& name, const Graph& rhs) const;
    Graph extract(const std::string& name, const Vector< std::string >& vertices) const;

  private:
    std::string name_;

    HashTable< std::string, bool, StringHash, StringEqual > vertices_;
    HashTable< std::string, Vector< Edge >, StringHash, StringEqual > outbound_;
    HashTable< std::string, Vector< Edge >, StringHash, StringEqual > inbound_;
  };
}

#endif
