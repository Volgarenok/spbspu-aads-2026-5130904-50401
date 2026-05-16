#include <fstream>
#include <iostream>
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

int main(int args, char ** argv)
{
  using namespace chernov;

  if (args != 2) {
    std::cerr << "filename is required, count of args must be 1\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "could not open file\n";
    return 1;
  }

  std::istream & input = std::cin;
  std::ostream & output = std::cout;

  Graphs graphs;

  std::string graph_name;
  while (file >> graph_name) {
    size_t edges_count;
    file >> edges_count;
    for (size_t i = 0; i < edges_count; ++i) {
      std::string start, end;
      size_t weight;
      file >> start >> end >> weight;
      graphs.addEdge(graph_name, start, end, weight);
    }
  }
}

chernov::Graphs::Graphs():
  graphs_(),
  capacity_(64)
{
  graphs_.rehash(capacity_);
}

void chernov::Graphs::addEdge(std::string graph_name, std::string start_vertex, std::string end_vertex, size_t weight)
{
  if (!graphs_.has(graph_name)) {
    try {
      graphs_.add(graph_name, Graph(graph_name));
    } catch (const std::length_error & e) {
      capacity_ *= 2;
      graphs_.rehash(capacity_);
      graphs_.add(graph_name, Graph(graph_name));
    }
  }
  graphs_.at(graph_name).addEdge(start_vertex, end_vertex, weight);
}

chernov::Graph::Graph(std::string name):
  name_(name),
  incoming_(),
  outgoing_(),
  incoming_capacity_(64),
  outgoing_capacity_(64)
{
  incoming_.rehash(incoming_capacity_);
  outgoing_.rehash(outgoing_capacity_);
}

void chernov::Graph::addEdge(std::string start_vertex, std::string end_vertex, size_t weight)
{
  if (!incoming_.has(end_vertex)) {
    try {
      incoming_.add(end_vertex, Edges());
    } catch (const std::length_error & e) {
      incoming_capacity_ *= 2;
      incoming_.rehash(incoming_capacity_);
      incoming_.add(end_vertex, Edges());
    }
  }
  if (!outgoing_.has(end_vertex)) {
    try {
      outgoing_.add(end_vertex, Edges());
    } catch (const std::length_error & e) {
      outgoing_capacity_ *= 2;
      outgoing_.rehash(outgoing_capacity_);
      outgoing_.add(end_vertex, Edges());
    }
  }
  incoming_.at(end_vertex).addEdge(start_vertex, weight);
  outgoing_.at(start_vertex).addEdge(end_vertex, weight);
}

chernov::Edges::Edges():
  edges_(),
  capacity_(64)
{
  edges_.rehash(capacity_);
}

void chernov::Edges::addEdge(std::string vertex, size_t weight)
{
  if (!edges_.has(vertex)) {
    try {
      edges_.add(vertex, Vector< size_t >());
    } catch (const std::length_error & e) {
      capacity_ *= 2;
      edges_.rehash(capacity_);
      edges_.add(vertex, Vector< size_t >());
    }
  }
  edges_.at(vertex).pushBack(weight);
}
