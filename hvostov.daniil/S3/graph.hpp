#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <cstddef>
#include <string>
#include <utility>
#include <top-it-vector.hpp>
#include "hash_table.hpp"
#include "sip_hash.hpp"

namespace hvostov {
  using edge_key = std::pair< std::string, std::string >;

  struct EdgeKeyEqual {
    bool operator()(const edge_key& a, const edge_key& b) const
    {
      return a.first == b.first && a.second == b.second;
    }
  };

  struct Graph {
    Graph() = default;
    ~Graph() = default;

    void addVertex(const std::string& v);
    void removeVertex(const std::string& v);
    bool hasVertex(const std::string& v) const;

    void addEdge(const std::string& from, const std::string& to, size_t w);
    void removeEdge(const std::string& from, const std::string& to, size_t w);
    bool hasEdge(const std::string& from, const std::string& to) const;

    Vector< std::string > vertices_;
    HashTable< edge_key, Vector< size_t >, SipHash< edge_key >, EdgeKeyEqual > edges_;
  };

}

#endif
