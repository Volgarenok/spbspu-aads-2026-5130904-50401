#include "graph.hpp"

bool studilova::PairEqual::operator()(const std::pair< std::string, std::string>& lhs,
  const std::pair< std::string, std::string >& rhs) const
{
  return lhs.first == rhs.first && lhs.second == rhs.second;
}

studilova::Graph::Graph(size_t edgeCapacity):
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
  if (tmp.edges_.contains(key))
  {
    Weights weights = tmp.edges_.at(key);
    weights.pushBack(weight);
    tmp.edges_.insert(std::pair< EdgeKey, Weights >(key, weights));
  }
  else
  {
    Weights weights;
    weights.pushBack(weight);
    tmp.edges_.insert(std::pair< EdgeKey, Weights >(key, weights));
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
  if (!edges_.contains(key))
  {
    return false;
  }

  Graph tmp(*this);
  Weights weights = tmp.edges_.at(key);
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
  }
  else
  {
    tmp.edges_.insert(std::pair< EdgeKey, Weights >(key, weights));
  }

  swap(tmp);
  return true;
}

const studilova::Vector< std::string >& studilova::Graph::getVertices() const
{
  return vertices_;
}

const studilova::Graph::EdgeTable& studilova::Graph::getEdges() const
{
  return edges_;
}

studilova::Graph::Connections studilova::Graph::getOutbound(const std::string& vertex) const
{
  if (!hasVertex(vertex))
  {
    throw std::out_of_range("Vertex not found");
  }

  Connections result;
  for (auto it = edges_.begin(); it != edges_.end(); ++it)
  {
    auto edge = *it;

    const EdgeKey& key = edge.first;
    const Weights& weights = edge.second;

    if (key.first == vertex)
    {
      result.pushBack(std::pair< std::string, Weights>(key.second, weights));
    }
  }
  return result;
}

studilova::Graph::Connections studilova::Graph::getInbound(const std::string& vertex) const
{
  if (!hasVertex(vertex))
  {
    throw std::out_of_range("Vertex not found");
  }

  Connections result;
  for (auto it = edges_.begin(); it != edges_.end(); ++it)
  {
    auto edge = *it;

    const EdgeKey& key = edge.first;
    const Weights& weights = edge.second;

    if (key.second == vertex)
    {
      result.pushBack(std::pair< std::string, Weights>(key.first, weights));
    }
  }
  return result;
}
