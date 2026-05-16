#include "graph.hpp"

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
