#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <cstddef>
#include <string>

#include <hashtable.hpp>
#include <vector.hpp>

namespace chernov {
  struct Edges {
    HashTable< std::string, Vector< size_t >, std::hash< std::string >, std::equal_to< std::string > > edges_;

    Edges();
    void addEdge(std::string vertex, size_t weight);
  };

  struct Graph {
    std::string name_;
    HashTable< std::string, Edges, std::hash< std::string >, std::equal_to< std::string > > incoming_;
    HashTable< std::string, Edges, std::hash< std::string >, std::equal_to< std::string > > outgoing_;

    Graph() = delete;
    Graph(std::string name);
    void addEdge(std::string start_vertex, std::string end_vertex, size_t weight);
  };

  struct Graphs {
    HashTable< std::string, Graph, std::hash< std::string >, std::equal_to< std::string > > graphs_;

    Graphs();
    void addEdge(std::string graph_name, std::string start_vertex, std::string end_vertex, size_t weight);
  };
}

#endif
