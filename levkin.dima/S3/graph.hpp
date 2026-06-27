#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <cstddef>
#include <iostream>
#include <string>
#include <utility>

#include "hasher.hpp"
#include "hashtable.hpp"
#include "vector.hpp"

namespace levkin {

  struct KeyComp {
    bool operator()(const std::string& p1, const std::string& p2) const
    {
      return p1 == p2;
    }
  };

  template < class T > struct Comp {
    bool operator()(const T& p1, const T& p2) { return p1 < p2; }
  };

  template < class F, class S > struct PairComp {
    using pair_t = std::pair< F, S >;
    bool operator()(const pair_t& p1, const pair_t& p2)
    {
      if (p1.first != p2.first) {
        return p1.first < p2.first;
      }
      return p1.second < p2.second;
    }
  };

  template < class T, class Cmp > void sort(stuff::Vector< T >& v, Cmp cmp);

  struct Edges {
    HashTable<
        std::string,
        stuff::Vector< size_t >,
        Sha1Hasher< std::string >,
        KeyComp >
        edges_;

    Edges();
    stuff::Vector< std::pair< std::string, size_t > > getEdges() const;
    void addEdge(std::string vertex, size_t weight);
    void cutEdge(std::string vertex, size_t weight);
  };

  struct Graph {
    std::string name_;
    HashTable< std::string, Edges, Sha1Hasher< std::string >, KeyComp >
        incoming_;
    HashTable< std::string, Edges, Sha1Hasher< std::string >, KeyComp >
        outgoing_;

    Graph() = default;
    Graph(std::string name);
    void addVertex(std::string vertex);
    void
    addEdge(std::string start_vertex, std::string end_vertex, size_t weight);
    void
    cutEdge(std::string start_vertex, std::string end_vertex, size_t weight);
    stuff::Vector< std::string > getVertexes() const;
    stuff::Vector< std::pair< std::string, size_t > >
    getOutbound(std::string vertex) const;
    stuff::Vector< std::pair< std::string, size_t > >
    getInbound(std::string vertex) const;
  };

  struct DB {
    DB();
    HashTable< std::string, Graph, Sha1Hasher< std::string >, KeyComp > graphs_;

    void
    addVertex(std::string graph_name, std::string vertex, std::ostream& output);
    void createGraphUnsafe(std::string graph_name);
    void createGraph(std::string graph_name);
    bool hasGraph(const std::string& name) const;
    void addEdge(
        std::string graph_name,
        std::string start_vertex,
        std::string end_vertex,
        size_t weight);
    void showGraphs(std::ostream& output);
    void showGraphVertexes(std::string graph_name, std::ostream& output);
    void showGraphEdges(
        stuff::Vector< std::pair< std::string, size_t > >& edges,
        std::ostream& output);
    void showGraphOutbound(
        std::string graph_name, std::string vertex, std::ostream& output);
    void showGraphInbound(
        std::string graph_name, std::string vertex, std::ostream& output);
    void bindGraphVertexes(
        std::string graph_name,
        std::string vertex_a,
        std::string vertex_b,
        size_t weight,
        std::ostream& output);
    void cutGraphEdge(
        std::string graph_name,
        std::string vertex_a,
        std::string vertex_b,
        size_t weight,
        std::ostream& output);
    void mergeGraphs(
        std::string new_graph,
        std::string old_graph1,
        std::string old_graph2,
        std::ostream& output);
    void extractGraphs(
        std::string new_graph,
        std::string old_graph,
        size_t count_k,
        stuff::Vector< std::string >& vertexes,
        std::ostream& output);
  };
}

#endif
