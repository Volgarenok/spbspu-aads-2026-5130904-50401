#include "Hash-table.hpp"
#include "hash-structs.hpp"

namespace donkeev
{
  class Graph
  {
    size_t edgesCount_;
    size_t vertexCount_;
    donkeev::HashTable< std::pair< std::string, std::string >, topit::Vector< size_t >, donkeev::VertexPairHash, donkeev::VertexPairEqual > table_;
  };
}
