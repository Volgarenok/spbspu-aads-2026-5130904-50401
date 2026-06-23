#include "graph.hpp"

#include <stdexcept>
#include <utility>

void kondrat::Graph::addVertex(const std::string & vertex)
{
  if (!vertexes_.contains(vertex))
  {
    vertexes_.add(vertex, true);
  }
}

bool kondrat::Graph::hasVertex(const std::string & vertex) const
{
  return vertexes_.contains(vertex);
}

void kondrat::Graph::bind(const std::string & from, const std::string & to, size_t weight)
{
  Graph copy(*this);
  copy.addVertex(from);
  copy.addVertex(to);

  const EdgeKey key(from, to);
  if (copy.edges_.contains(key))
  {
    copy.edges_.at(key).pushBack(weight);
  }
  else
  {
    WeightList weights;
    weights.pushBack(weight);
    copy.edges_.add(key, std::move(weights));
  }
  swap(copy);
}

void kondrat::Graph::cut(const std::string & from, const std::string & to, size_t weight)
{
  if (!hasVertex(from) || !hasVertex(to))
  {
    throw std::logic_error("invalid graph operation");
  }

  const EdgeKey key(from, to);
  if (!edges_.contains(key))
  {
    throw std::logic_error("invalid graph operation");
  }

  Graph copy(*this);
  WeightList & weights = copy.edges_.at(key);
  bool found = false;
  for (size_t i = 0; i < weights.getSize(); ++i)
  {
    if (weights[i] == weight)
    {
      weights.erase(i);
      found = true;
      break;
    }
  }
  if (!found)
  {
    throw std::logic_error("invalid graph operation");
  }
  if (weights.isEmpty())
  {
    copy.edges_.erase(key);
  }
  swap(copy);
}

kondrat::Vector< std::string > kondrat::Graph::getVertexes() const
{
  Vector< std::string > result;
  HTCIter< std::string, bool > it = vertexes_.begin();
  const HTCIter< std::string, bool > end = vertexes_.end();
  while (it != end)
  {
    result.pushBack(it->first);
    ++it;
  }
  return result;
}

kondrat::Vector< kondrat::EdgeInfo >
kondrat::Graph::getOutbound(const std::string & vertex) const
{
  if (!hasVertex(vertex))
  {
    throw std::logic_error("invalid graph operation");
  }

  Vector< EdgeInfo > result;
  HTCIter< EdgeKey, WeightList > it = edges_.begin();
  const HTCIter< EdgeKey, WeightList > end = edges_.end();
  while (it != end)
  {
    if (it->first.first == vertex)
    {
      result.pushBack(EdgeInfo(it->first.second, it->second));
    }
    ++it;
  }
  return result;
}

kondrat::Vector< kondrat::EdgeInfo >
kondrat::Graph::getInbound(const std::string & vertex) const
{
  if (!hasVertex(vertex))
  {
    throw std::logic_error("invalid graph operation");
  }

  Vector< EdgeInfo > result;
  HTCIter< EdgeKey, WeightList > it = edges_.begin();
  const HTCIter< EdgeKey, WeightList > end = edges_.end();
  while (it != end)
  {
    if (it->first.second == vertex)
    {
      result.pushBack(EdgeInfo(it->first.first, it->second));
    }
    ++it;
  }
  return result;
}

void kondrat::Graph::mergeFrom(const Graph & first, const Graph & second)
{
  Graph copy;
  const Vector< std::string > firstVertexes = first.getVertexes();
  const Vector< std::string > secondVertexes = second.getVertexes();
  for (size_t i = 0; i < firstVertexes.getSize(); ++i)
  {
    copy.addVertex(firstVertexes[i]);
  }
  for (size_t i = 0; i < secondVertexes.getSize(); ++i)
  {
    copy.addVertex(secondVertexes[i]);
  }

  HTCIter< EdgeKey, WeightList > firstIt = first.edges_.begin();
  const HTCIter< EdgeKey, WeightList > firstEnd = first.edges_.end();
  while (firstIt != firstEnd)
  {
    for (size_t i = 0; i < firstIt->second.getSize(); ++i)
    {
      copy.bind(firstIt->first.first, firstIt->first.second, firstIt->second[i]);
    }
    ++firstIt;
  }

  HTCIter< EdgeKey, WeightList > secondIt = second.edges_.begin();
  const HTCIter< EdgeKey, WeightList > secondEnd = second.edges_.end();
  while (secondIt != secondEnd)
  {
    for (size_t i = 0; i < secondIt->second.getSize(); ++i)
    {
      copy.bind(secondIt->first.first, secondIt->first.second, secondIt->second[i]);
    }
    ++secondIt;
  }
  swap(copy);
}

void kondrat::Graph::extractFrom(const Graph & graph, const Vector< std::string > & vertexes)
{
  Graph copy;
  for (size_t i = 0; i < vertexes.getSize(); ++i)
  {
    if (!graph.hasVertex(vertexes[i]))
    {
      throw std::logic_error("invalid graph operation");
    }
    copy.addVertex(vertexes[i]);
  }

  HTCIter< EdgeKey, WeightList > it = graph.edges_.begin();
  const HTCIter< EdgeKey, WeightList > end = graph.edges_.end();
  while (it != end)
  {
    if (copy.hasVertex(it->first.first) && copy.hasVertex(it->first.second))
    {
      for (size_t i = 0; i < it->second.getSize(); ++i)
      {
        copy.bind(it->first.first, it->first.second, it->second[i]);
      }
    }
    ++it;
  }
  swap(copy);
}

void kondrat::Graph::swap(Graph & graph) noexcept
{
  vertexes_.swap(graph.vertexes_);
  edges_.swap(graph.edges_);
}
