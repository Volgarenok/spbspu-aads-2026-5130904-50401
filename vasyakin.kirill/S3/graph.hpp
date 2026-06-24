#ifndef GRAPH_HPP
#define GRAPH_HPP
#include "hash_table.hpp"
#include "hmac_hash.hpp"
#include <string>
#include <functional>

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
  };
}

#endif
