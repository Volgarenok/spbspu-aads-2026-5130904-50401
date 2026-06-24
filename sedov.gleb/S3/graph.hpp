#ifndef GRAPH_HPP
#define GRAPH_HPP
#include "../common/list.hpp"
#include "hash_table.hpp"
#include "siphash.hpp"
#include <string>
#include <utility>
#include <stdexcept>

namespace sedov
{
  class Graph
  {
  public:
    using keyPair = std::pair< std::string, std::string >;
    using listWeights = sedov::List< size_t >;

    Graph() noexcept;
    explicit Graph(size_t slots);

    void addVertex(const std::string & v);
    void removeVertex(const std::string & v);
    void addEdge(const std::string & from, const std::string & to, size_t weight);
    void removeEdge(const std::string & from, const std::string & to, size_t weight);

    sedov::List< std::string > vertices;
    HashTable< keyPair, listWeights, sedov::PairHasher< std::string >, std::equal_to< keyPair > > edges;
  };
}

#endif
