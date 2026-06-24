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
  bundle.popBack();
}
stuff::Vector< std::pair< std::string, size_t > >
levkin::Edges::getEdges() const
{
  stuff::Vector< std::pair< std::string, size_t > > pipeline;
  for (auto it = edges_.cbegin(); it != edges_.cend(); ++it) {
    if (it->is_valid_) {
      const std::string& target = it->key_;
      const stuff::Vector< size_t >& weights = it->value_;
      for (size_t i = 0; i < weights.getSize(); ++i) {
        pipeline.pushBack(std::make_pair(target, weights[i]));
      }
    }
  }
  return pipeline;
}
levkin::Graph::Graph(std::string name) :
  name_(name), incoming_(64, 4), outgoing_(64, 4)
{
}
void levkin::Graph::addVertex(std::string vertex)
{
  if (!outgoing_.has(vertex)) {
    outgoing_.add(vertex, Edges());
  }
}
void levkin::Graph::addEdge(std::string start_vertex,
                            std::string end_vertex,
                            size_t weight)
{
  bool added_incoming_node = false;
  bool added_outgoing_node = false;
  if (!incoming_.has(end_vertex)) {
    incoming_.add(end_vertex, Edges());
    added_incoming_node = true;
  }
  if (!outgoing_.has(start_vertex)) {
    outgoing_.add(start_vertex, Edges());
    added_outgoing_node = true;
  }
  try {
    incoming_.at(end_vertex).addEdge(start_vertex, weight);
    outgoing_.at(start_vertex).addEdge(end_vertex, weight);
  } catch (...) {
    if (added_incoming_node) {
      incoming_.drop(end_vertex);
    }
    if (added_outgoing_node) {
      outgoing_.drop(start_vertex);
    }
    if (!added_incoming_node) {
      try {
        incoming_.at(end_vertex).cutEdge(start_vertex, weight);
      } catch (...) {
      }
    }
    throw;
  }
}
void levkin::Graph::cutEdge(std::string start_vertex,
                            std::string end_vertex,
                            size_t weight)
{
  incoming_.at(end_vertex).cutEdge(start_vertex, weight);
  outgoing_.at(start_vertex).cutEdge(end_vertex, weight);
}
stuff::Vector< std::string > levkin::Graph::getVertexes() const
{
  stuff::Vector< std::string > container;
  for (auto it = outgoing_.cbegin(); it != outgoing_.cend(); ++it) {
    if (it->is_valid_)
      container.pushBack(it->key_);
  }
  for (auto it = incoming_.cbegin(); it != incoming_.cend(); ++it) {
    if (it->is_valid_ && !outgoing_.has(it->key_)) {
      container.pushBack(it->key_);
    }
  }
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
  if (out_check) {
    return outgoing_.at(vertex).getEdges();
  }
  return stuff::Vector< std::pair< std::string, size_t > >();
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
void levkin::DB::addVertex(std::string graph_name,
                           std::string vertex,
                           std::ostream&)
{
  graphs_.at(graph_name).addVertex(vertex);
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
void levkin::DB::addEdge(std::string graph_name,
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
  for (auto it = graphs_.cbegin(); it != graphs_.cend(); ++it) {
    if (it->is_valid_) {
      keys.pushBack(it->key_);
    }
  }
  size_t len = keys.getSize();
  if (len == 0) {
    return;
  }
  sort(keys, Comp< std::string >{});
  for (size_t i = 0; i < len; ++i) {
    output << keys[i] << "\n";
  }
}
void levkin::DB::showGraphVertexes(std::string graph_name, std::ostream& output)
{
  stuff::Vector< std::string > collection
      = graphs_.at(graph_name).getVertexes();
  sort(collection, Comp< std::string >{});
  size_t cap = collection.getSize();
  for (size_t i = 0; i < cap; ++i) {
    output << collection[i] << "\n";
  }
}
void levkin::DB::showGraphEdges(
    stuff::Vector< std::pair< std::string, size_t > >& edges,
    std::ostream& output)
{
  size_t total = edges.getSize();
  if (total == 0)
    return;
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
void levkin::DB::showGraphOutbound(std::string graph_name,
                                   std::string vertex,
                                   std::ostream& output)
{
  stuff::Vector< std::pair< std::string, size_t > > paths
      = graphs_.at(graph_name).getOutbound(vertex);
  showGraphEdges(paths, output);
}
void levkin::DB::showGraphInbound(std::string graph_name,
                                  std::string vertex,
                                  std::ostream& output)
{
  stuff::Vector< std::pair< std::string, size_t > > paths
      = graphs_.at(graph_name).getInbound(vertex);
  showGraphEdges(paths, output);
}
void levkin::DB::bindGraphVertexes(std::string graph_name,
                                   std::string vertex_a,
                                   std::string vertex_b,
                                   size_t weight,
                                   std::ostream&)
{
  graphs_.at(graph_name).addEdge(vertex_a, vertex_b, weight);
}
void levkin::DB::cutGraphEdge(std::string graph_name,
                              std::string vertex_a,
                              std::string vertex_b,
                              size_t weight,
                              std::ostream&)
{
  graphs_.at(graph_name).cutEdge(vertex_a, vertex_b, weight);
}
void levkin::DB::mergeGraphs(std::string new_graph,
                             std::string old_graph1,
                             std::string old_graph2,
                             std::ostream&)
{
  if (graphs_.has(new_graph) || !graphs_.has(old_graph1)
      || !graphs_.has(old_graph2)) {
    throw std::out_of_range("invalid graphs");
  }
  Graph union_set(new_graph);
  auto append_graph = [&](const std::string& g_name) {
    const Graph& g = graphs_.at(g_name);
    for (auto it = g.outgoing_.cbegin(); it != g.outgoing_.cend(); ++it) {
      if (it->is_valid_) {
        union_set.addVertex(it->key_);
      }
    }
    for (auto it = g.incoming_.cbegin(); it != g.incoming_.cend(); ++it) {
      if (it->is_valid_) {
        union_set.addVertex(it->key_);
      }
    }
    for (auto it = g.outgoing_.cbegin(); it != g.outgoing_.cend(); ++it) {
      if (it->is_valid_) {
        const std::string& src = it->key_;
        const Edges& edge_container = it->value_;
        for (auto e_it = edge_container.edges_.cbegin();
             e_it != edge_container.edges_.cend();
             ++e_it) {
          if (e_it->is_valid_) {
            const std::string& dest = e_it->key_;
            const stuff::Vector< size_t >& weights = e_it->value_;
            for (size_t w = 0; w < weights.getSize(); ++w) {
              union_set.addEdge(src, dest, weights[w]);
            }
          }
        }
      }
    }
  };
  append_graph(old_graph1);
  append_graph(old_graph2);
  graphs_.add(new_graph, union_set);
}
void levkin::DB::extractGraphs(std::string new_graph,
                               std::string old_graph,
                               size_t count_k,
                               stuff::Vector< std::string >& vertexes,
                               std::ostream&)
{
  if (graphs_.has(new_graph) || !graphs_.has(old_graph)) {
    throw std::out_of_range("invalid graphs");
  }
  const Graph& source_ref = graphs_.at(old_graph);
  size_t boundary = count_k < vertexes.getSize() ? count_k : vertexes.getSize();
  levkin::HashTable< std::string, bool > valid_verts(64, 4);
  for (size_t i = 0; i < boundary; ++i) {
    if (!source_ref.outgoing_.has(vertexes[i])
        && !source_ref.incoming_.has(vertexes[i])) {
      throw std::out_of_range("vertex not in graph");
    }
    valid_verts.add(vertexes[i], true);
  }
  Graph subsection(new_graph);
  for (size_t i = 0; i < boundary; ++i) {
    subsection.addVertex(vertexes[i]);
  }
  for (size_t i = 0; i < boundary; ++i) {
    const std::string& start_pt = vertexes[i];
    if (source_ref.outgoing_.has(start_pt)) {
      const Edges& edge_container = source_ref.outgoing_.at(start_pt);
      for (auto e_it = edge_container.edges_.cbegin();
           e_it != edge_container.edges_.cend();
           ++e_it) {
        if (e_it->is_valid_) {
          const std::string& destination_pt = e_it->key_;
          if (valid_verts.has(destination_pt)) {
            const stuff::Vector< size_t >& weights = e_it->value_;
            for (size_t w = 0; w < weights.getSize(); ++w) {
              subsection.addEdge(start_pt, destination_pt, weights[w]);
            }
          }
        }
      }
    }
  }
  graphs_.add(new_graph, subsection);
}
