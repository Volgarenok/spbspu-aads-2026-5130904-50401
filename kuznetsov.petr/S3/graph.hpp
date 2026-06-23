#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <cstddef>
#include <string>
#include <utility>

#include <top-it-vector.hpp>

#include "HashTable.hpp"
#include "Hasher.hpp"
namespace kuznetsov {
  using key = std::pair< std::string, std::string >;

  struct KeyComparator {
    bool operator()(const key& e1, const key& e2) const
    {
      bool f = e1.first == e2.first;
      f = f && e1.second == e2.second;
      return f;
    }

    bool operator()(const std::string& s1, const std::string& s2) const
    {
      return s1 == s2;
    }
  };

  struct Graph {
    Graph();
    Graph(size_t c);

    void addVertexes(const std::string& name);
    void removeVertexes(const std::string& name);
    void addEdge(const std::string& v1, const std::string& v2, size_t w);
    void removeEdge(const std::string& v1, const std::string& v2, size_t w);

    HashTable< key, Vector< size_t >, SipHasher< key >, KeyComparator > table_;
    Vector< std::string > vertexes_;
  };
}

#endif
