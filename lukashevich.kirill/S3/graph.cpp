#include "graph.hpp"

#include <cstddef>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <functional>

namespace {
  const std::size_t GRAPH_BUCKET_COUNT = 251;
  const std::size_t GRAPH_BUCKET_SIZE = 8;

  struct EdgeInfo
  {
    std::string vertex_;
    lukashevich::WeightVector weights_;
  };

  struct EdgeInfoLess
  {
    bool operator()(const EdgeInfo & lhs, const EdgeInfo & rhs) const
    {
      return lhs.vertex_ < rhs.vertex_;
    }
  };

  template< class T, class Compare >
  void selectionSort(lukashevich::Vector< T > & values, Compare compare)
  {
    for (size_t i = 0; i < values.getSize(); ++i) {
      size_t min = i;

      for (size_t j = i + 1; j < values.getSize(); ++j) {
        if (compare(values[j], values[min])) {
          min = j;
        }
      }

      if (min != i) {
        std::swap(values[i], values[min]);
      }
    }
  }

  bool hasWeight(const lukashevich::WeightVector & weights, lukashevich::Weight weight)
  {
    for (size_t i = 0; i < weights.getSize(); ++i) {
      if (weights[i] == weight) {
        return true;
      }
    }

    return false;
  }

  void removeWeight(lukashevich::WeightVector & weights, lukashevich::Weight weight)
  {
    for (size_t i = 0; i < weights.getSize(); ++i) {
      if (weights[i] == weight) {
        weights.erase(i);
        return;
      }
    }

    throw std::logic_error("weight not found");
  }

  void printWeights(lukashevich::WeightVector weights, std::ostream & out)
  {
    selectionSort(weights, std::less< lukashevich::Weight >());

    for (size_t i = 0; i < weights.getSize(); ++i) {
      out << ' ' << weights[i];
    }
  }

  void printEdgeInfo(lukashevich::Vector< EdgeInfo > & info, std::ostream & out)
  {
    selectionSort(info, EdgeInfoLess());

    for (size_t i = 0; i < info.getSize(); ++i) {
      if (i != 0) {
        out << '\n';
      }

      out << info[i].vertex_;
      printWeights(info[i].weights_, out);
    }
  }
}

lukashevich::Graph::Graph():
  vertexes_(GRAPH_BUCKET_COUNT, GRAPH_BUCKET_SIZE),
  edges_(GRAPH_BUCKET_COUNT, GRAPH_BUCKET_SIZE)
{}

bool lukashevich::Graph::hasVertex(const std::string & vertex) const
{
  return vertexes_.has(vertex);
}

void lukashevich::Graph::addVertex(const std::string & vertex)
{
  Graph copy(*this);
  copy.addVertexUnsafe(vertex);
  swap(copy);
}

void lukashevich::Graph::bind(const std::string & from, const std::string & to, Weight weight)
{
  Graph copy(*this);

  copy.addVertexUnsafe(from);
  copy.addVertexUnsafe(to);
  copy.bindUnsafe(from, to, weight);

  swap(copy);
}

void lukashevich::Graph::cut(const std::string & from, const std::string & to, Weight weight)
{
  Graph copy(*this);
  copy.cutUnsafe(from, to, weight);
  swap(copy);
}

void lukashevich::Graph::swap(Graph & rhs) noexcept
{
  vertexes_.swap(rhs.vertexes_);
  edges_.swap(rhs.edges_);
}

const lukashevich::Graph::VertexTable & lukashevich::Graph::getVertexes() const noexcept
{
  return vertexes_;
}

const lukashevich::Graph::EdgeTable &
    lukashevich::Graph::getEdges() const noexcept
{
  return edges_;
}

void lukashevich::Graph::printVertexes(std::ostream & out) const
{
  Vector< std::string > names;

  for (VertexTable::const_iterator it = vertexes_.cbegin();
      it != vertexes_.cend();
      ++it) {
    names.pushBack(it->first);
  }

  selectionSort(names, std::less< std::string >());

  for (size_t i = 0; i < names.getSize(); ++i) {
    if (i != 0) {
      out << '\n';
    }
    out << names[i];
  }
}

void lukashevich::Graph::printOutbound(const std::string & vertex, std::ostream & out) const
{
  if (!hasVertex(vertex)) {
    throw std::logic_error("vertex not found");
  }

  Vector< EdgeInfo > result;

  for (EdgeTable::const_iterator it = edges_.cbegin();
      it != edges_.cend();
      ++it) {
    if (it->first.first == vertex) {
      EdgeInfo info;
      info.vertex_ = it->first.second;
      info.weights_ = it->second;
      result.pushBack(info);
    }
  }

  printEdgeInfo(result, out);
}

void lukashevich::Graph::printInbound(const std::string & vertex, std::ostream & out) const
{
  if (!hasVertex(vertex)) {
    throw std::logic_error("vertex not found");
  }

  Vector< EdgeInfo > result;

  for (EdgeTable::const_iterator it = edges_.cbegin();
      it != edges_.cend();
      ++it) {
    if (it->first.second == vertex) {
      EdgeInfo info;
      info.vertex_ = it->first.first;
      info.weights_ = it->second;
      result.pushBack(info);
    }
  }

  printEdgeInfo(result, out);
}

void lukashevich::Graph::addVertexUnsafe(const std::string & vertex)
{
  if (!vertexes_.has(vertex)) {
    vertexes_.add(vertex, true);
  }
}

void lukashevich::Graph::bindUnsafe(const std::string & from,
    const std::string & to,
    Weight weight)
{
  const EdgeKey edge(from, to);

  if (edges_.has(edge)) {
    edges_.at(edge).pushBack(weight);
  } else {
    WeightVector weights;
    weights.pushBack(weight);
    edges_.add(edge, weights);
  }
}

void lukashevich::Graph::cutUnsafe(const std::string & from,
    const std::string & to,
    Weight weight)
{
  if (!hasVertex(from) || !hasVertex(to)) {
    throw std::logic_error("vertex not found");
  }

  const EdgeKey edge(from, to);

  if (!edges_.has(edge) || !hasWeight(edges_.at(edge), weight)) {
    throw std::logic_error("edge not found");
  }

  WeightVector & weights = edges_.at(edge);
  removeWeight(weights, weight);

  if (weights.isEmpty()) {
    edges_.erase(edge);
  }
}

void lukashevich::addGraphCopy(GraphTable & graphs, const std::string & name, const Graph & graph)
{
  graphs.add(name, graph);
}
