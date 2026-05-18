#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <cstddef>
#include <string>
#include <utility>
#include "../common/list.hpp"
#include "../common/myVector.hpp"
#include "myHash.hpp"
#include "hasher.hpp"

namespace zhuravleva
{
  class Graph
  {
    private:
      myVector< std::string > vertexes;
      HashTable< std::pair< std::string, std::string >, List< size_t >,
        Blake2Hasher< std::pair< std::string, std::string > >, KeyEqual > edges;
    public:
      Graph(size_t bucket_count = 16);
      bool hasVertex(const std::string& vertex) const;
      void addVertex(const std::string& vertex);
      void bind(const std::string& a, const std::string& b, size_t weight);
      bool cut(const std::string& a, const std::string& b, size_t weight);
      const myVector< std::string >& getVertexes() const;

      HashTable< std::pair< std::string, std::string >, List< size_t >,
        Blake2Hasher< std::pair< std::string, std::string > >, KeyEqual >& getEdges();

      const HashTable< std::pair< std::string, std::string >, List< size_t >,
        Blake2Hasher< std::pair< std::string, std::string > >, KeyEqual >& getEdges() const;
    };
}

#endif