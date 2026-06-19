#include "graph.hpp"

#include <cstddef>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace lukashevich {
  namespace detail {
    const size_t GRAPH_BUCKET_COUNT = 251;
    const size_t GRAPH_BUCKET_SIZE = 8;

    struct StringLess
    {
      bool operator()(const std::string & lhs, const std::string & rhs) const
      {
        return lhs < rhs;
      }
    };

    struct WeightLess
    {
      bool operator()(Weight lhs, Weight rhs) const
      {
        return lhs < rhs;
      }
    };

    struct EdgeInfo
    {
      std::string vertex_;
      WeightVector weights_;
    };

    struct EdgeInfoLess
    {
      bool operator()(const EdgeInfo & lhs, const EdgeInfo & rhs) const
      {
        return lhs.vertex_ < rhs.vertex_;
      }
    };

    template< class T, class Compare >
    void selectionSort(Vector< T > & values, Compare compare)
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

    bool hasWeight(const WeightVector & weights, Weight weight)
    {
      for (size_t i = 0; i < weights.getSize(); ++i) {
        if (weights[i] == weight) {
          return true;
        }
      }

      return false;
    }

    void removeWeight(WeightVector & weights, Weight weight)
    {
      for (size_t i = 0; i < weights.getSize(); ++i) {
        if (weights[i] == weight) {
          weights.erase(i);
          return;
        }
      }

      throw std::logic_error("weight not found");
    }

    void printWeights(WeightVector weights, std::ostream & out)
    {
      selectionSort(weights, WeightLess());

      for (size_t i = 0; i < weights.getSize(); ++i) {
        out << ' ' << weights[i];
      }
    }

    void printEdgeInfo(Vector< EdgeInfo > & info, std::ostream & out)
    {
      selectionSort(info, EdgeInfoLess());

      if (info.isEmpty()) {
        out << '\n';
        return;
      }

      for (size_t i = 0; i < info.getSize(); ++i) {
        out << info[i].vertex_;
        printWeights(info[i].weights_, out);
        out << '\n';
      }
    }
  }
}

lukashevich::Graph::Graph():
  vertexes_(detail::GRAPH_BUCKET_COUNT, detail::GRAPH_BUCKET_SIZE),
  edges_(detail::GRAPH_BUCKET_COUNT, detail::GRAPH_BUCKET_SIZE)
{}

bool lukashevich::Graph::hasVertex(const std::string & vertex) const
{
  return vertexes_.has(vertex);
}

void lukashevich::Graph::addVertex(const std::string & vertex)
{
  if (!vertexes_.has(vertex)) {
    vertexes_.add(vertex, true);
  }
}

void lukashevich::Graph::bind(const std::string & from, const std::string & to, Weight weight)
{
  addVertex(from);
  addVertex(to);

  const EdgeKey edge(from, to);

  if (edges_.has(edge)) {
    edges_.at(edge).pushBack(weight);
  } else {
    WeightVector weights;
    weights.pushBack(weight);
    edges_.add(edge, weights);
  }
}

void lukashevich::Graph::cut(const std::string & from, const std::string & to, Weight weight)
{
  if (!hasVertex(from) || !hasVertex(to)) {
    throw std::logic_error("vertex not found");
  }

  const EdgeKey edge(from, to);

  if (!edges_.has(edge) || !detail::hasWeight(edges_.at(edge), weight)) {
    throw std::logic_error("edge not found");
  }

  WeightVector & weights = edges_.at(edge);
  detail::removeWeight(weights, weight);

  if (weights.isEmpty()) {
    edges_.drop(edge);
  }
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
    names.pushBack(it->key_);
  }

  detail::selectionSort(names, detail::StringLess());

  if (names.isEmpty()) {
    out << '\n';
    return;
  }

  for (size_t i = 0; i < names.getSize(); ++i) {
    out << names[i] << '\n';
  }
}

void lukashevich::Graph::printOutbound(const std::string & vertex, std::ostream & out) const
{
  if (!hasVertex(vertex)) {
    throw std::logic_error("vertex not found");
  }

  Vector< detail::EdgeInfo > result;

  for (EdgeTable::const_iterator it = edges_.cbegin();
      it != edges_.cend();
      ++it) {
    if (it->key_.first == vertex) {
      detail::EdgeInfo info;
      info.vertex_ = it->key_.second;
      info.weights_ = it->value_;
      result.pushBack(info);
    }
  }

  detail::printEdgeInfo(result, out);
}

void lukashevich::Graph::printInbound(const std::string & vertex, std::ostream & out) const
{
  if (!hasVertex(vertex)) {
    throw std::logic_error("vertex not found");
  }

  Vector< detail::EdgeInfo > result;

  for (EdgeTable::const_iterator it = edges_.cbegin();
      it != edges_.cend();
      ++it) {
    if (it->key_.second == vertex) {
      detail::EdgeInfo info;
      info.vertex_ = it->key_.first;
      info.weights_ = it->value_;
      result.pushBack(info);
    }
  }

  detail::printEdgeInfo(result, out);
}

void lukashevich::addGraphCopy(GraphTable & graphs, const std::string & name, const Graph & graph)
{
  graphs.add(name, graph);
}
