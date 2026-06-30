#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <cstddef>
#include <functional>
#include <string>
#include <utility>
#include <top-it-vector.hpp>
#include "hash_table.hpp"
#include "sip_hash.hpp"

namespace hvostov {
  using edge_key = std::pair< std::string, std::string >;

  struct Graph {
    Vector< std::string > vertices_;
    HashTable< edge_key, Vector< size_t >, SipHash< edge_key >, std::equal_to< edge_key > > edges_;

    Graph() = default;
    ~Graph() = default;

    void swap(Graph& other) noexcept;
    void addVertex(const std::string& v);
    void removeVertex(const std::string& v);
    bool hasVertex(const std::string& v) const;

    void addEdge(const std::string& from, const std::string& to, size_t w);
    void removeEdge(const std::string& from, const std::string& to, size_t w);
    bool hasEdge(const std::string& from, const std::string& to) const;
  };

}

#endif
