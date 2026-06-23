#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <cstddef>
#include <string>

#include "hasher.hpp"
#include <hashtable.hpp>
#include <vector.hpp>

namespace chernov {
  struct KeyComparator {
    bool operator()(const std::string & s1, const std::string & s2) const
    {
      return s1 == s2;
    }
  };

  template< class T >
  struct Comparator {
    bool operator()(const T & p1, const T & p2)
    {
      return p1 < p2;
    }
  };

  template< class F, class S >
  struct PairComparator {
    using pair_t = std::pair< F, S >;
    bool operator()(const pair_t & p1, const pair_t & p2)
    {
      if (p1.first != p2.first) {
        return p1.first < p2.first;
      }
      return p1.second < p2.second;
    }
  };

  template< class T, class Cmp >
  void sort(Vector< T > & v, Cmp cmp);

  struct Edges {
    HashTable< std::string, Vector< size_t >, HasherXx< std::string >, KeyComparator > edges_;

    Edges();
    void addEdge(std::string vertex, size_t weight);
    void cutEdge(std::string vertex, size_t weight);
    Vector< std::pair< std::string, size_t > > getEdges() const;
  };

  struct Graph {
    std::string name_;
    HashTable< std::string, Edges, HasherXx< std::string >, KeyComparator > incoming_;
    HashTable< std::string, Edges, HasherXx< std::string >, KeyComparator > outgoing_;

    Graph() = delete;
    Graph(std::string name);
    void addVertex(std::string vertex);
    void addEdge(std::string start_vertex, std::string end_vertex, size_t weight);
    void cutEdge(std::string start_vertex, std::string end_vertex, size_t weight);
    Vector< std::string > getVertexes() const;
    Vector< std::pair< std::string, size_t > > getOutbound(std::string vertex) const;
    Vector< std::pair< std::string, size_t > > getInbound(std::string vertex) const;
  };

  struct Graphs {
    HashTable< std::string, Graph, HasherXx< std::string >, KeyComparator > graphs_;

    Graphs();
    void addVertex(std::string graph_name, std::string vertex, std::ostream & output);
    void createGraphWithoutCheckingExisting(std::string graph_name);
    void createGraph(std::string graph_name);
    bool hasGraph(const std::string& name) const;
    void addEdge(std::string graph_name, std::string start_vertex, std::string end_vertex, size_t weight);
    void showGraphs(std::ostream & output);
    void showGraphVertexes(std::string graph_name, std::ostream & output);
    void showGraphEdges(Vector< std::pair< std::string, size_t > > & edges, std::ostream & output);
    void showGraphOutbound(std::string graph_name, std::string vertex, std::ostream & output);
    void showGraphInbound(std::string graph_name, std::string vertex, std::ostream & output);
    void bindGraphVertexes(std::string graph_name, std::string vertex_a, std::string vertex_b, size_t weight, std::ostream & output);
    void cutGraphEdge(std::string graph_name, std::string vertex_a, std::string vertex_b, size_t weight, std::ostream & output);
    void mergeGraphs(std::string new_graph, std::string old_graph1, std::string old_graph2, std::ostream & output);
    void extractGraphs(std::string new_graph, std::string old_graph, size_t count_k, Vector< std::string > & vertexes, std::ostream & output);
  };
}

#endif
