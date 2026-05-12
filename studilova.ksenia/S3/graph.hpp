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

      explicit Graph(size_t edgeCapacity = 64);

    private:
      studilova::Vector< std::string> vertices_;
      EdgeTable edges_;
  };
}

#endif
