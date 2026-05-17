#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <cstddef>
#include <string>

#include <hashtable.hpp>
#include <vector.hpp>

namespace chernov {
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
    HashTable< std::string, Vector< size_t >, std::hash< std::string >, std::equal_to< std::string > > edges_;

    Edges();
    void addEdge(std::string vertex, size_t weight);
    Vector< std::pair< std::string, size_t > > getEdges() const;
  };

  struct Graph {
    std::string name_;
    HashTable< std::string, Edges, std::hash< std::string >, std::equal_to< std::string > > incoming_;
    HashTable< std::string, Edges, std::hash< std::string >, std::equal_to< std::string > > outgoing_;

    Graph() = delete;
    Graph(std::string name);
    void addEdge(std::string start_vertex, std::string end_vertex, size_t weight);
    Vector< std::string > getVertexes() const;
    Vector< std::pair< std::string, size_t > > getOutbound(std::string vertex) const;
    Vector< std::pair< std::string, size_t > > getInbound(std::string vertex) const;
  };

  struct Graphs {
    HashTable< std::string, Graph, std::hash< std::string >, std::equal_to< std::string > > graphs_;

    Graphs();
    void addEdge(std::string graph_name, std::string start_vertex, std::string end_vertex, size_t weight);
    void showGraphs(std::ostream & output);
    void showGraphVertexes(std::string graph_name, std::ostream & output);
    void showGraphOutbound(std::string graph_name, std::string vertex, std::ostream & output);
    void showGraphInbound(std::string graph_name, std::string vertex, std::ostream & output);
  };
}

#endif
