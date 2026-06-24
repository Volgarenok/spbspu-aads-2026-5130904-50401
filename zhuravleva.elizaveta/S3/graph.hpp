#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <cstddef>
#include <functional>
#include <string>
#include <utility>
#include "list.hpp"
#include "myVector.hpp"
#include "myHash.hpp"
#include "hasher.hpp"

namespace zhuravleva
{
  using EdgeKey = std::pair< std::string, std::string >;
  using EdgeTable = HashTable< EdgeKey, List< size_t >,
      Blake2Hasher< EdgeKey >, std::equal_to< EdgeKey > >;

  class Graph
  {
    public:
      Graph(size_t bucket_count = 16);
      bool hasVertex(const std::string& vertex) const;
      void addVertex(const std::string& vertex);
      void bind(const std::string& a, const std::string& b, size_t weight);
      bool cut(const std::string& a, const std::string& b, size_t weight);
      const myVector< std::string >& getVertexes() const;
      EdgeTable& getEdges();
      const EdgeTable& getEdges() const;
      void swap(Graph& other) noexcept;

    private:
      myVector< std::string > vertexes_;
      EdgeTable edges_;
    };
}

#endif
