#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <cstddef>
#include <string>
#include <hashtable.hpp>
#include <vector.hpp>
#include "hasher.hpp"

namespace chernov {
  namespace detail {
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
  }

  struct KeyComparator {
    bool operator()(const std::string & s1, const std::string & s2) const
    {
      return std::equal_to< std::string >{}(s1, s2);
    }
  };

  template< class T >
  struct Comparator {
    bool operator()(const T & p1, const T & p2)
    {
      return std::less< T >{}(p1, p2);
    }
  };

  struct Edges {
    HashTable< std::string, Vector< size_t >, HasherXx< std::string >, KeyComparator > edges_;

    Edges();
    void addEdge(const std::string & vertex, size_t weight);
    void cutEdge(const std::string & vertex, size_t weight);
    Vector< std::pair< std::string, size_t > > getEdges() const;
  };

  struct Graph {
    std::string name_;
    HashTable< std::string, Edges, HasherXx< std::string >, KeyComparator > incoming_;
    HashTable< std::string, Edges, HasherXx< std::string >, KeyComparator > outgoing_;

    Graph() = delete;
    Graph(const std::string & name);
    void addVertex(const std::string & vertex);
    void addEdge(const std::string & start_vertex, const std::string & end_vertex, size_t weight);
    void cutEdge(const std::string & start_vertex, const std::string & end_vertex, size_t weight);
    Vector< std::string > getVertexes() const;
    Vector< std::pair< std::string, size_t > > getOutbound(const std::string & vertex) const;
    Vector< std::pair< std::string, size_t > > getInbound(const std::string & vertex) const;
  };

  struct Graphs {
    HashTable< std::string, Graph, HasherXx< std::string >, KeyComparator > graphs_;

    Graphs();
    void addVertex(const std::string & graph_name, const std::string & vertex, std::ostream & output);
    void createGraphWithoutCheckingExisting(const std::string & graph_name);
    void createGraph(const std::string & graph_name);
    bool hasGraph(const std::string & name) const;
    void addEdge(const std::string & graph_name,
      const std::string & start_vertex,
      const std::string & end_vertex,
      size_t weight);
    void showGraphs(std::ostream & output);
    void showGraphVertexes(const std::string & graph_name, std::ostream & output);
    void showGraphEdges(Vector< std::pair< std::string, size_t > > & edges, std::ostream & output);
    void showGraphOutbound(const std::string & graph_name, const std::string & vertex, std::ostream & output);
    void showGraphInbound(const std::string & graph_name, const std::string & vertex, std::ostream & output);
    void bindGraphVertexes(const std::string & graph_name,
      const std::string & vertex_a,
      const std::string & vertex_b,
      size_t weight,
      std::ostream & output);
    void cutGraphEdge(const std::string & graph_name,
      const std::string & vertex_a,
      const std::string & vertex_b,
      size_t weight,
      std::ostream & output);
    void mergeGraphs(const std::string & new_graph,
      const std::string & old_graph1,
      const std::string & old_graph2,
      std::ostream & output);
    void extractGraphs(const std::string & new_graph,
      const std::string & old_graph,
      size_t count_k,
      Vector< std::string > & vertexes,
      std::ostream & output);
  };
}

#endif
