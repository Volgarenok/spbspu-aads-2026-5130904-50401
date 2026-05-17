#include "graph.hpp"

chernov::Edges::Edges():
  edges_(64)
{}

void chernov::Edges::addEdge(std::string vertex, size_t weight)
{
  if (!edges_.has(vertex)) {
    try {
      edges_.add(vertex, Vector< size_t >());
    } catch (const std::length_error & e) {
      edges_.rehash(edges_.maxCapacity() * 2);
      edges_.add(vertex, Vector< size_t >());
    }
  }
  edges_.at(vertex).pushBack(weight);
}

chernov::Graph::Graph(std::string name):
  name_(name),
  incoming_(64),
  outgoing_(64)
{}

void chernov::Graph::addEdge(std::string start_vertex, std::string end_vertex, size_t weight)
{
  if (!incoming_.has(end_vertex)) {
    try {
      incoming_.add(end_vertex, Edges());
    } catch (const std::length_error & e) {
      incoming_.rehash(incoming_.maxCapacity() * 2);
      incoming_.add(end_vertex, Edges());
    }
  }
  if (!outgoing_.has(start_vertex)) {
    try {
      outgoing_.add(start_vertex, Edges());
    } catch (const std::length_error & e) {
      outgoing_.rehash(outgoing_.maxCapacity() * 2);
      outgoing_.add(start_vertex, Edges());
    }
  }
  incoming_.at(end_vertex).addEdge(start_vertex, weight);
  outgoing_.at(start_vertex).addEdge(end_vertex, weight);
}

chernov::Graphs::Graphs():
  graphs_(64)
{}

void chernov::Graphs::addEdge(std::string graph_name, std::string start_vertex, std::string end_vertex, size_t weight)
{
  if (!graphs_.has(graph_name)) {
    try {
      graphs_.add(graph_name, Graph(graph_name));
    } catch (const std::length_error & e) {
      graphs_.rehash(graphs_.maxCapacity() * 2);
      graphs_.add(graph_name, Graph(graph_name));
    }
  }
  graphs_.at(graph_name).addEdge(start_vertex, end_vertex, weight);
}
