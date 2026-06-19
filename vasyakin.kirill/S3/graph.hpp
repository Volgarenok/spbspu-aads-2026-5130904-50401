#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <string>
#include <functional>
#include "hash_table.hpp"
#include "hmac_hash.hpp"

namespace vasyakin
{
  struct Edge
  {
    std::string to;
    size_t weight;
  };

  struct Graph
  {
    HashTable< std::string, vasyakin::List< Edge >, vasyakin::HMACHash, std::equal_to< std::string > > adj;

    Graph() noexcept;

    void addVertex(const std::string& vertex);
    void addEdge(const std::string& from, const std::string& to, size_t weight);
    bool hasVertex(const std::string& vertex) const;
    bool hasEdge(const std::string& from, const std::string& to, size_t weight) const;
    void removeEdge(const std::string& from, const std::string& to, size_t weight);

    vasyakin::Vector< std::string > getAllVertices() const;

    vasyakin::Vector< std::pair< std::string, size_t > > getOutgoingEdges(
      const std::string& vertex) const;

    vasyakin::Vector< std::pair< std::string, size_t > > getIncomingEdges(
      const std::string& vertex) const;
  };
}

#endif
