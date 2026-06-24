#include "graph.hpp"
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
levkin::Graph::Graph(std::string name) :
  name_(name), edges_(64, 4), vertices_(64, 4)
{
}
void levkin::Graph::addVertex(const std::string& vertex)
{
  if (!vertices_.has(vertex)) {
    vertices_.add(vertex, true);
  }
}
void levkin::Graph::addEdge(const std::string& start_vertex,
                            const std::string& end_vertex,
                            size_t weight)
{
  bool had_start = vertices_.has(start_vertex);
  bool had_end = vertices_.has(end_vertex);
  try {
    addVertex(start_vertex);
    addVertex(end_vertex);
    PairKey key{start_vertex, end_vertex};
    if (!edges_.has(key)) {
      edges_.add(key, stuff::Vector< size_t >());
    }
    edges_.at(key).pushBack(weight);
  } catch (...) {
    if (!had_start) {
      vertices_.remove(start_vertex);
    }
    if (!had_end) {
      vertices_.remove(end_vertex);
    }
    throw;
  }
}
void levkin::Graph::cutEdge(const std::string& start_vertex,
                            const std::string& end_vertex,
                            size_t weight)
{
  if (!vertices_.has(start_vertex) || !vertices_.has(end_vertex)) {
    throw std::out_of_range("vertex not found");
  }
  PairKey key{start_vertex, end_vertex};
  if (!edges_.has(key)) {
    throw std::out_of_range("edge not found");
  }
  stuff::Vector< size_t >& bundle = edges_.at(key);
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
  if (bundle.getSize() == 0) {
    edges_.remove(key);
  }
}
stuff::Vector< std::string > levkin::Graph::getVertexes() const
{
  stuff::Vector< std::string > container;
  for (auto it = vertices_.begin(); it != vertices_.end(); ++it) {
    if (it->valid) {
      container.pushBack(it->key);
    }
  }
  return container;
}
stuff::Vector< std::pair< std::string, size_t > >
levkin::Graph::getOutbound(const std::string& vertex) const
{
  if (!vertices_.has(vertex)) {
    throw std::out_of_range("vertex not found");
  }
  stuff::Vector< std::pair< std::string, size_t > > pipeline;
  for (auto it = edges_.begin(); it != edges_.end(); ++it) {
    if (it->valid && it->key.from == vertex) {
      const std::string& to_vertex = it->key.to;
      const stuff::Vector< size_t >& weights = it->value;
      for (size_t i = 0; i < weights.getSize(); ++i) {
        pipeline.pushBack({to_vertex, weights[i]});
      }
    }
  }
  return pipeline;
}
stuff::Vector< std::pair< std::string, size_t > >
levkin::Graph::getInbound(const std::string& vertex) const
{
  if (!vertices_.has(vertex)) {
    throw std::out_of_range("vertex not found");
  }
  stuff::Vector< std::pair< std::string, size_t > > pipeline;
  for (auto it = edges_.begin(); it != edges_.end(); ++it) {
    if (it->valid && it->key.to == vertex) {
      const std::string& from_vertex = it->key.from;
      const stuff::Vector< size_t >& weights = it->value;
      for (size_t i = 0; i < weights.getSize(); ++i) {
        pipeline.pushBack({from_vertex, weights[i]});
      }
    }
  }
  return pipeline;
}
levkin::DB::DB() : graphs_(64, 4) {}
void levkin::DB::showGraphs(std::ostream& output) const
{
  stuff::Vector< std::string > keys;
  for (auto it = graphs_.begin(); it != graphs_.end(); ++it) {
    if (it->valid) {
      keys.pushBack(it->key);
    }
  }
  sort(keys, Comp< std::string >());
  for (size_t i = 0; i < keys.getSize(); ++i) {
    output << keys[i];
    if (i + 1 < keys.getSize()) {
      output << "\n";
    }
  }
}
void levkin::DB::showGraphVertexes(const std::string& graphName,
                                   std::ostream& output) const
{
  const Graph& graph = graphs_.at(graphName);
  stuff::Vector< std::string > verts = graph.getVertexes();
  sort(verts, Comp< std::string >());
  output << graph.name_;
  for (size_t i = 0; i < verts.getSize(); ++i) {
    output << " " << verts[i];
  }
}
void levkin::DB::showGraphOutbound(const std::string& graphName,
                                   const std::string& vertex,
                                   std::ostream& output) const
{
  const Graph& graph = graphs_.at(graphName);
  auto edges = graph.getOutbound(vertex);
  sort(edges, PairComp< std::string, size_t >());
  for (size_t i = 0; i < edges.getSize(); ++i) {
    output << vertex << " " << edges[i].first << " " << edges[i].second;
    if (i + 1 < edges.getSize()) {
      output << "\n";
    }
  }
}
void levkin::DB::showGraphInbound(const std::string& graphName,
                                  const std::string& vertex,
                                  std::ostream& output) const
{
  const Graph& graph = graphs_.at(graphName);
  auto edges = graph.getInbound(vertex);
  sort(edges, PairComp< std::string, size_t >());
  for (size_t i = 0; i < edges.getSize(); ++i) {
    output << edges[i].first << " " << vertex << " " << edges[i].second;
    if (i + 1 < edges.getSize()) {
      output << "\n";
    }
  }
}
void levkin::DB::addVertex(const std::string& graphName,
                           const std::string& vertex,
                           std::ostream&)
{
  graphs_.at(graphName).addVertex(vertex);
}
void levkin::DB::bindGraphVertexes(const std::string& graphName,
                                   const std::string& vertexA,
                                   const std::string& vertexB,
                                   size_t weight,
                                   std::ostream&)
{
  if (!graphs_.has(graphName)) {
    graphs_.add(graphName, Graph(graphName));
  }
  graphs_.at(graphName).addEdge(vertexA, vertexB, weight);
}
void levkin::DB::cutGraphEdge(const std::string& graphName,
                              const std::string& vertexA,
                              const std::string& vertexB,
                              size_t weight,
                              std::ostream&)
{
  graphs_.at(graphName).cutEdge(vertexA, vertexB, weight);
}
void levkin::DB::mergeGraphs(const std::string& newGraph,
                             const std::string& oldGraph1,
                             const std::string& oldGraph2,
                             std::ostream&)
{
  if (graphs_.has(newGraph) || !graphs_.has(oldGraph1)
      || !graphs_.has(oldGraph2)) {
    throw std::invalid_argument("invalid graphs for merge");
  }
  Graph unionSet(newGraph);
  const Graph& g1 = graphs_.at(oldGraph1);
  const Graph& g2 = graphs_.at(oldGraph2);
  stuff::Vector< std::string > v1 = g1.getVertexes();
  stuff::Vector< std::string > v2 = g2.getVertexes();
  for (size_t i = 0; i < v1.getSize(); ++i) {
    unionSet.addVertex(v1[i]);
  }
  for (size_t i = 0; i < v2.getSize(); ++i) {
    unionSet.addVertex(v2[i]);
  }
  for (auto it = g1.edges_.begin(); it != g1.edges_.end(); ++it) {
    if (it->valid) {
      for (size_t i = 0; i < it->value.getSize(); ++i) {
        unionSet.addEdge(it->key.from, it->key.to, it->value[i]);
      }
    }
  }
  for (auto it = g2.edges_.begin(); it != g2.edges_.end(); ++it) {
    if (it->valid) {
      for (size_t i = 0; i < it->value.getSize(); ++i) {
        unionSet.addEdge(it->key.from, it->key.to, it->value[i]);
      }
    }
  }
  graphs_.add(newGraph, unionSet);
}
void levkin::DB::extractGraphs(const std::string& newGraph,
                               const std::string& oldGraph,
                               size_t countK,
                               stuff::Vector< std::string >& vertexes,
                               std::ostream&)
{
  if (graphs_.has(newGraph) || !graphs_.has(oldGraph)) {
    throw std::invalid_argument("invalid graphs for extract");
  }
  Graph subsection(newGraph);
  const Graph& sourceRef = graphs_.at(oldGraph);
  size_t boundary = countK < vertexes.getSize() ? countK : vertexes.getSize();
  for (size_t i = 0; i < boundary; ++i) {
    if (sourceRef.vertices_.has(vertexes[i])) {
      subsection.addVertex(vertexes[i]);
    }
  }
  for (auto it = sourceRef.edges_.begin(); it != sourceRef.edges_.end(); ++it) {
    if (it->valid) {
      bool fromMatch = false;
      bool toMatch = false;
      for (size_t c = 0; c < boundary; ++c) {
        if (it->key.from == vertexes[c]) {
          fromMatch = true;
        }
        if (it->key.to == vertexes[c]) {
          toMatch = true;
        }
      }
      if (fromMatch && toMatch) {
        for (size_t j = 0; j < it->value.getSize(); ++j) {
          subsection.addEdge(it->key.from, it->key.to, it->value[j]);
        }
      }
    }
  }
  graphs_.add(newGraph, subsection);
}

void levkin::DB::addEdge(const std::string& graphName,
                         const std::string& startVertex,
                         const std::string& endVertex,
                         size_t weight)
{
  if (!graphs_.has(graphName)) {
    graphs_.add(graphName, Graph(graphName));
  }
  graphs_.at(graphName).addEdge(startVertex, endVertex, weight);
}

void levkin::DB::createGraphUnsafe(const std::string& graphName)
{
  graphs_.add(graphName, Graph(graphName));
}

void levkin::DB::createGraph(const std::string& graphName)
{
  if (!graphs_.has(graphName)) {
    createGraphUnsafe(graphName);
  }
}

bool levkin::DB::hasGraph(const std::string& name) const
{
  return graphs_.has(name);
}
