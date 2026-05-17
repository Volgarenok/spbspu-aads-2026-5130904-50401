#include "graph.hpp"

#include <iostream>

template< class T, class Cmp >
void chernov::sort(Vector< T > & v, Cmp cmp)
{
  for (size_t i = 0; i < v.getSize() - 1; ++i) {
    size_t min = i;
    for (size_t j = i + 1; j < v.getSize(); ++j) {
      if (cmp(v[j], v[min])) {
        min = j;
      }
    }
    if (min != i) {
      std::swap(v[min], v[i]);
    }
  }
}

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

chernov::Vector< std::pair< std::string, size_t > > chernov::Edges::getEdges() const
{
  Vector< std::pair< std::string, size_t > > edges;
  for (auto ht_iter = edges_.cbegin(); ht_iter != edges_.cend(); ++ht_iter) {
    for (auto v_iter = ht_iter->second.cbegin(); v_iter != ht_iter->second.cend(); ++v_iter) {
      edges.pushBack({ht_iter->first, *v_iter});
    }
  }
  return edges;
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

chernov::Vector< std::string > chernov::Graph::getVertexes() const
{
  Vector< std::string > vertexes;
  for (auto iter = incoming_.cbegin(); iter != incoming_.cend(); ++iter) {
    vertexes.pushBack(iter->first);
  }
  for (auto iter = outgoing_.cbegin(); iter != outgoing_.cend(); ++iter) {
    bool flag = true;
    for (auto v_iter = vertexes.cbegin(); v_iter != vertexes.cend(); ++v_iter) {
      if (iter->first == *v_iter) {
        flag = false;
        break;
      }
    }
    if (flag) {
      vertexes.pushBack(iter->first);
    }
  }
  return vertexes;
}

chernov::Vector< std::pair< std::string, size_t > > chernov::Graph::getOutbound(std::string vertex) const
{
  return outgoing_.at(vertex).getEdges();
}

chernov::Vector< std::pair< std::string, size_t > > chernov::Graph::getInbound(std::string vertex) const
{
  return incoming_.at(vertex).getEdges();
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

void chernov::Graphs::showGraphs(std::ostream & output)
{
  for (auto iter = graphs_.begin(); iter != graphs_.end(); ++iter) {
    output << iter->first << "\n";
  }
}

void chernov::Graphs::showGraphVertexes(std::string graph_name, std::ostream & output)
{
  try {
    Vector< std::string > vertexes = graphs_.at(graph_name).getVertexes();
    sort(vertexes, Comparator< std::string >{});
    for (auto iter = vertexes.cbegin(); iter != vertexes.cend(); ++iter) {
      output << *iter << "\n";
    }
  } catch (const std::out_of_range & e) {
    output << "<INVALID COMMAND>\n";
  }
}

void chernov::Graphs::showGraphOutbound(std::string graph_name, std::string vertex, std::ostream & output)
{
  try {
    Vector< std::pair< std::string, size_t > > vertexes = graphs_.at(graph_name).getOutbound(vertex);
    sort(vertexes, PairComparator< std::string, size_t >{});
    for (auto iter = vertexes.cbegin(); iter != vertexes.cend(); ++iter) {
      output << iter->first << " " << iter->second << "\n";
    }
  } catch (const std::out_of_range & e) {
    output << "<INVALID COMMAND>\n";
  }
}

void chernov::Graphs::showGraphInbound(std::string graph_name, std::string vertex, std::ostream & output)
{
  try {
    Vector< std::pair< std::string, size_t > > vertexes = graphs_.at(graph_name).getInbound(vertex);
    sort(vertexes, PairComparator< std::string, size_t >{});
    for (auto iter = vertexes.cbegin(); iter != vertexes.cend(); ++iter) {
      output << iter->first << " " << iter->second << "\n";
    }
  } catch (const std::out_of_range & e) {
    output << "<INVALID COMMAND>\n";
  }
}
