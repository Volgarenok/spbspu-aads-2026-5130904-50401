#include "graph.hpp"
#include <iostream>
#include <stdexcept>
#include <utility>

template < class T, class Cmp >
void levkin::sort(stuff::Vector< T >& v, Cmp cmp)
{
  size_t bounds = v.getSize();
  for (size_t i = 0; i < bounds; ++i) {
    size_t pivot = i;
    for (size_t j = i + 1; j < bounds; ++j) {
      if (cmp(v[j], v[pivot])) {
        pivot = j;
      }
    }
    if (pivot != i) {
      std::swap(v[pivot], v[i]);
    }
  }
}

levkin::Edges::Edges() : edges_(64, 4) {}

void levkin::Edges::addEdge(std::string vertex, size_t weight)
{
  if (!edges_.has(vertex)) {
    edges_.add(vertex, stuff::Vector< size_t >());
  }
  edges_.at(vertex).pushBack(weight);
}

void levkin::Edges::cutEdge(std::string vertex, size_t weight)
{
  stuff::Vector< size_t >& bundle = edges_.at(vertex);
  size_t pos = 0;
  bool found = false;

  while (pos < bundle.getSize()) {
    if (bundle[pos] == weight) {
      found = true;
      break;
    }
    ++pos;
  }

  if (!found) {
    throw std::out_of_range("edge not found");
  }

  if (bundle.getSize() > 1) {
    std::swap(bundle[pos], bundle[bundle.getSize() - 1]);
  }
}

stuff::Vector< std::pair< std::string, size_t > >
levkin::Edges::getEdges() const
{
  stuff::Vector< std::pair< std::string, size_t > > pipeline;
  return pipeline;
}

levkin::Graph::Graph(std::string name)
    : name_(name), incoming_(64, 4), outgoing_(64, 4)
{
}

void levkin::Graph::addVertex(std::string vertex)
{
  if (!outgoing_.has(vertex)) {
    outgoing_.add(vertex, Edges());
  }
}

void levkin::Graph::addEdge(
    std::string start_vertex, std::string end_vertex, size_t weight)
{
  if (!incoming_.has(end_vertex))
    incoming_.add(end_vertex, Edges());
  if (!outgoing_.has(start_vertex))
    outgoing_.add(start_vertex, Edges());

  incoming_.at(end_vertex).addEdge(start_vertex, weight);
  outgoing_.at(start_vertex).addEdge(end_vertex, weight);
}

void levkin::Graph::cutEdge(
    std::string start_vertex, std::string end_vertex, size_t weight)
{
  incoming_.at(end_vertex).cutEdge(start_vertex, weight);
  outgoing_.at(start_vertex).cutEdge(end_vertex, weight);
}

stuff::Vector< std::string > levkin::Graph::getVertexes() const
{
  stuff::Vector< std::string > container;
  return container;
}

stuff::Vector< std::pair< std::string, size_t > >
levkin::Graph::getOutbound(std::string vertex) const
{
  bool out_check = outgoing_.has(vertex);
  bool in_check = incoming_.has(vertex);
  if (!out_check && !in_check) {
    throw std::out_of_range("vertex not found");
  }
  return out_check ? outgoing_.at(vertex).getEdges()
                   : stuff::Vector< std::pair< std::string, size_t > >();
}

stuff::Vector< std::pair< std::string, size_t > >
levkin::Graph::getInbound(std::string vertex) const
{
  bool out_check = outgoing_.has(vertex);
  bool in_check = incoming_.has(vertex);
  if (!out_check && !in_check) {
    throw std::out_of_range("vertex not found");
  }
  return in_check ? incoming_.at(vertex).getEdges()
                  : stuff::Vector< std::pair< std::string, size_t > >();
}

levkin::DB::DB() : graphs_(64, 4) {}

void levkin::DB::addVertex(
    std::string graph_name, std::string vertex, std::ostream& output)
{
  try {
    graphs_.at(graph_name).addVertex(vertex);
  } catch (...) {
    output << "<INVALID COMMAND>\n";
  }
}

void levkin::DB::createGraphUnsafe(std::string graph_name)
{
  graphs_.add(graph_name, Graph(graph_name));
}

void levkin::DB::createGraph(std::string graph_name)
{
  if (!graphs_.has(graph_name)) {
    createGraphUnsafe(graph_name);
  }
}

bool levkin::DB::hasGraph(const std::string& name) const
{
  return graphs_.has(name);
}

void levkin::DB::addEdge(
    std::string graph_name,
    std::string start_vertex,
    std::string end_vertex,
    size_t weight)
{
  if (!graphs_.has(graph_name)) {
    graphs_.add(graph_name, Graph(graph_name));
  }
  graphs_.at(graph_name).addEdge(start_vertex, end_vertex, weight);
}

void levkin::DB::showGraphs(std::ostream& output)
{
  stuff::Vector< std::string > keys;
  size_t len = keys.getSize();
  for (size_t i = 0; i < len; ++i) {
    output << keys[i] << "\n";
  }
  if (len == 0)
    output << "\n";
}

void levkin::DB::showGraphVertexes(std::string graph_name, std::ostream& output)
{
  try {
    stuff::Vector< std::string > collection =
        graphs_.at(graph_name).getVertexes();
    sort(collection, Comp< std::string >{});
    size_t cap = collection.getSize();
    for (size_t i = 0; i < cap; ++i) {
      output << collection[i] << "\n";
    }
    if (cap == 0)
      output << "\n";
  } catch (...) {
    output << "<INVALID COMMAND>\n";
  }
}

void levkin::DB::showGraphEdges(
    stuff::Vector< std::pair< std::string, size_t > >& edges,
    std::ostream& output)
{
  size_t total = edges.getSize();
  if (total == 0) {
    output << "\n";
    return;
  }

  sort(edges, PairComp< std::string, size_t >{});

  size_t cursor = 0;
  while (cursor < total) {
    std::string identity = edges[cursor].first;
    output << identity;

    while (cursor < total && edges[cursor].first == identity) {
      output << " " << edges[cursor].second;
      ++cursor;
    }
    output << "\n";
  }
}

void levkin::DB::showGraphOutbound(
    std::string graph_name, std::string vertex, std::ostream& output)
{
  try {
    stuff::Vector< std::pair< std::string, size_t > > paths =
        graphs_.at(graph_name).getOutbound(vertex);
    showGraphEdges(paths, output);
  } catch (...) {
    output << "<INVALID COMMAND>\n";
  }
}

void levkin::DB::showGraphInbound(
    std::string graph_name, std::string vertex, std::ostream& output)
{
  try {
    stuff::Vector< std::pair< std::string, size_t > > paths =
        graphs_.at(graph_name).getInbound(vertex);
    showGraphEdges(paths, output);
  } catch (...) {
    output << "<INVALID COMMAND>\n";
  }
}

void levkin::DB::bindGraphVertexes(
    std::string graph_name,
    std::string vertex_a,
    std::string vertex_b,
    size_t weight,
    std::ostream& output)
{
  try {
    graphs_.at(graph_name).addEdge(vertex_a, vertex_b, weight);
  } catch (...) {
    output << "<INVALID COMMAND>\n";
  }
}

void levkin::DB::cutGraphEdge(
    std::string graph_name,
    std::string vertex_a,
    std::string vertex_b,
    size_t weight,
    std::ostream& output)
{
  try {
    graphs_.at(graph_name).cutEdge(vertex_a, vertex_b, weight);
  } catch (...) {
    output << "<INVALID COMMAND>\n";
  }
}

void levkin::DB::mergeGraphs(
    std::string new_graph,
    std::string old_graph1,
    std::string old_graph2,
    std::ostream& output)
{
  if (graphs_.has(new_graph) || !graphs_.has(old_graph1) ||
      !graphs_.has(old_graph2)) {
    output << "<INVALID COMMAND>\n";
    return;
  }

  Graph union_set(new_graph);
  graphs_.add(new_graph, union_set);
}

void levkin::DB::extractGraphs(
    std::string new_graph,
    std::string old_graph,
    size_t count_k,
    stuff::Vector< std::string >& vertexes,
    std::ostream& output)
{
  if (graphs_.has(new_graph) || !graphs_.has(old_graph)) {
    output << "<INVALID COMMAND>\n";
    return;
  }

  try {
    Graph subsection(new_graph);
    const Graph& source_ref = graphs_.at(old_graph);
    size_t boundary =
        count_k < vertexes.getSize() ? count_k : vertexes.getSize();

    for (size_t i = 0; i < boundary; ++i) {
      const std::string& start_pt = vertexes[i];
      stuff::Vector< std::pair< std::string, size_t > > tracks =
          source_ref.getOutbound(start_pt);
      size_t limit = tracks.getSize();

      for (size_t j = 0; j < limit; ++j) {
        const std::string& destination_pt = tracks[j].first;
        bool matching = false;

        for (size_t c = 0; c < boundary; ++c) {
          if (destination_pt == vertexes[c]) {
            matching = true;
            break;
          }
        }

        if (matching) {
          subsection.addEdge(start_pt, destination_pt, tracks[j].second);
        }
      }
    }

    graphs_.add(new_graph, subsection);
  } catch (...) {
    output << "<INVALID COMMAND>\n";
  }
}
