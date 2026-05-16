#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <cstddef>
#include <string>

#include <hashtable.hpp>
#include <vector.hpp>

namespace chernov {
  class Edges {
  public:
    Edges();
    void addEdge(std::string vertex, size_t weight);
  private:
    HashTable< std::string, Vector< size_t >, std::hash< std::string >, std::equal_to< std::string > > edges_;
    size_t capacity_;
  };

  class Graph {
  public:
    Graph() = delete;
    Graph(std::string name);
    void addEdge(std::string start_vertex, std::string end_vertex, size_t weight);
  private:
    std::string name_;
    HashTable< std::string, Edges, std::hash< std::string >, std::equal_to< std::string > > incoming_;
    HashTable< std::string, Edges, std::hash< std::string >, std::equal_to< std::string > > outgoing_;
    size_t incoming_capacity_;
    size_t outgoing_capacity_;
  };

  class Graphs {
  public:
    Graphs();
    void addEdge(std::string graph_name, std::string start_vertex, std::string end_vertex, size_t weight);
  private:
    HashTable< std::string, Graph, std::hash< std::string >, std::equal_to< std::string > > graphs_;
    size_t capacity_;
  };
}

#endif
