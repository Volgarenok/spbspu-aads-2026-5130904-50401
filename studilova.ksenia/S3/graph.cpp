#include "graph.hpp"

bool studilova::PairEqual::operator()(
  const std::pair< std::string, std::string>& lhs,
  const std::pair< std::string, std::string >& rhs
) const
{
  return lhs.first == rhs.first && lhs.second == rhs.second;
}

studilova::Graph::Graph(size_t edgeCapacity) :
  vertices_(),
  edges_(edgeCapacity)
{}

bool studilova::Graph::hasVertex(const std::string& vertex) const
{
  for (size_t i = 0; i < vertices_.getSize(); ++i)
  {
    if (vertices_[i] == vertex)
    {
      return true;
    }
  }
  return false;
}

void studilova::Graph::addVertex(const std::string& vertex)
{
  if (!hasVertex(vertex))
  {
    vertices_.pushBack(vertex);
  }
}

void studilova::Graph::swap(Graph& other) noexcept
{
  vertices_.swap(other.vertices_);
  edges_.swap(other.edges_);
}

void studilova::Graph::bind(const std::string& from, const std::string& to, size_t weight)
{
  Graph tmp(*this);
  tmp.addVertex(from);
  tmp.addVertex(to);

  EdgeKey key(from, to);
  if (tmp.edges_.has(key))
  {
    Weights weights = tmp.edges_.get(key);
    weights.pushBack(weight);
    tmp.edges_.add(key, weights);
  } else {
    Weights weights;
    weights.pushBack(weight);
    tmp.edges_.add(key, weights);
  }

  swap(tmp);
}

bool studilova::Graph::cut(const std::string& from, const std::string& to, size_t weight)
{
  if (!hasVertex(from) || !hasVertex(to))
  {
    return false;
  }

  EdgeKey key(from, to);
  if (!edges_.has(key))
  {
    return false;
  }

  Graph tmp(*this);
  Weights weights = tmp.edges_.get(key);
  bool removed = false;

  for (size_t i = 0; i < weights.getSize(); ++i)
  {
    if (weights[i] == weight)
    {
      weights.erase(i);
      removed = true;
      break;
    }
  }

  if (!removed)
  {
    return false;
  }

  if (weights.isEmpty())
  {
    tmp.edges_.erase(key);
  } else {
    tmp.edges_.add(key, weights);
  }

  swap(tmp);
  return true;
}
