#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <cstddef>
#include <iosfwd>
#include <string>
#include <utility>
#include "hasher.hpp"
#include "hashtable.hpp"
#include "vector.hpp"
namespace levkin {
  struct PairKey
  {
    std::string from;
    std::string to;
    bool operator==(const PairKey& other) const
    {
      return from == other.from && to == other.to;
    }
  };
  struct PairKeyHasher
  {
    size_t operator()(const PairKey& key) const
    {
      Sha1Hasher< std::string > stringHasher;
      return stringHasher(key.from) ^ (stringHasher(key.to) << 1);
    }
  };
  struct KeyComp
  {
    bool operator()(const std::string& p1, const std::string& p2) const
    {
      return p1 == p2;
    }
  };
  template < class T >
  struct Comp
  {
    bool operator()(const T& p1, const T& p2) const { return p1 < p2; }
  };
  template < class F, class S >
  struct PairComp
  {
    using pair_t = std::pair< F, S >;
    bool operator()(const pair_t& p1, const pair_t& p2) const
    {
      if (p1.first != p2.first) {
        return p1.first < p2.first;
      }
      return p1.second < p2.second;
    }
  };
  template < class T, class Cmp >
  void sort(stuff::Vector< T >& v, Cmp cmp);
  struct Graph
  {
    std::string name_;
    HashTable< PairKey, stuff::Vector< size_t >, PairKeyHasher > edges_;
    HashTable< std::string, bool, Sha1Hasher< std::string >, KeyComp >
        vertices_;
    Graph() = default;
    explicit Graph(std::string name);
    void addVertex(const std::string& vertex);
    void addEdge(const std::string& startVertex,
                 const std::string& endVertex,
                 size_t weight);
    void cutEdge(const std::string& startVertex,
                 const std::string& endVertex,
                 size_t weight);
    stuff::Vector< std::string > getVertexes() const;
    stuff::Vector< std::pair< std::string, size_t > >
    getOutbound(const std::string& vertex) const;
    stuff::Vector< std::pair< std::string, size_t > >
    getInbound(const std::string& vertex) const;
  };
  struct DB
  {
    HashTable< std::string, Graph, Sha1Hasher< std::string >, KeyComp > graphs_;
    DB();
    void showGraphs(std::ostream& output) const;
    void addEdge(const std::string& graphName,
                 const std::string& startVertex,
                 const std::string& endVertex,
                 size_t weight);
    void showGraphVertexes(const std::string& graphName,
                           std::ostream& output) const;
    void showGraphOutbound(const std::string& graphName,
                           const std::string& vertex,
                           std::ostream& output) const;
    void showGraphInbound(const std::string& graphName,
                          const std::string& vertex,
                          std::ostream& output) const;
    void addVertex(const std::string& graphName,
                   const std::string& vertex,
                   std::ostream& output);
    void bindGraphVertexes(const std::string& graphName,
                           const std::string& vertexA,
                           const std::string& vertexB,
                           size_t weight,
                           std::ostream& output);
    void cutGraphEdge(const std::string& graphName,
                      const std::string& vertexA,
                      const std::string& vertexB,
                      size_t weight,
                      std::ostream& output);
    void mergeGraphs(const std::string& newGraph,
                     const std::string& oldGraph1,
                     const std::string& oldGraph2,
                     std::ostream& output);
    void extractGraphs(const std::string& newGraph,
                       const std::string& oldGraph,
                       size_t countK,
                       stuff::Vector< std::string >& vertexes,
                       std::ostream& output);
    void createGraphUnsafe(const std::string& graphName);
    void createGraph(const std::string& graphName);
    bool hasGraph(const std::string& name) const;
  };
}
#endif
