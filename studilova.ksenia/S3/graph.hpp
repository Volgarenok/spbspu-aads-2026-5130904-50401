#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "hash_table.hpp"
#include "hmac_hash.hpp"
#include "../common/vector.hpp"

#include <cstddef>
#include <string>
#include <utility>

namespace studilova
{
  struct PairEqual
  {
    bool operator()(
      const std::pair< std::string, std::string>& lhs,
      const std::pair< std::string, std::string>& rhs
    ) const;
  };

  class Graph
  {
    public:
      using EdgeKey = std::pair< std::string, std::string >;
      using Weights = studilova::Vector< size_t >;
      using EdgeTable = HashTable< EdgeKey, Weights, HMACHash, PairEqual >;
      using Connection = std::pair< std::string, Weights >;
      using Connections = Vector< Connection >;

      explicit Graph(size_t edgeCapacity = 64);

      bool hasVertex(const std::string& vertex) const;
      void addVertex(const std::string& vertex);

      void swap(Graph& other) noexcept;

      void bind(const std::string& from, const std::string& to, size_t weight);
      bool cut(const std::string& from, const std::string& to, size_t weight);

      const Vector< std::string>& getVertices() const;
      const EdgeTable& getEdges() const;

      Connections getOutbound(const std::string& vertex) const;
      Connections getInbound(const std::string& vertex) const;

    private:
      studilova::Vector< std::string> vertices_;
      EdgeTable edges_;
  };
}

#endif
