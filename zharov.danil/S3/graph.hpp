#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <functional>
#include <string>
#include <hash_table.hpp>
#include <vector.hpp>
#include "hasher.hpp"

namespace zharov
{
  using key_t = std::pair< std::string, std::string >;

  struct Graph
  {
    Graph() = default;
    Graph(size_t count);
    void addVertex(const std::string& v);
    void rmVertex(const std::string& v);
    void addEdge(const std::string& v1, const std::string& v2, size_t w);
    void rmEdge(const std::string& v1, const std::string& v2, size_t w);

    zharov::HashTable< key_t, Vector< size_t >, Blake2Hasher< key_t >, std::equal_to< key_t > >
      edges;
    zharov::Vector< std::string > vertexes;
  };
}

#endif
