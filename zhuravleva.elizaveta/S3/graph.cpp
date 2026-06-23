#include "graph.hpp"

zhuravleva::Graph::Graph(size_t bucket_count):
  vertexes_(),
  edges_(bucket_count)
{}

bool zhuravleva::Graph::hasVertex(const std::string& vertex) const
{
  for (auto it = vertexes_.cbegin(); it != vertexes_.cend(); ++it)
  {
    if (*it == vertex)
    {
      return true;
    }
  }
  return false;
}

void zhuravleva::Graph::addVertex(const std::string& vertex)
{
  if (!hasVertex(vertex))
  {
    vertexes_.pushBack(vertex);
  }
}

void zhuravleva::Graph::bind(const std::string& a, const std::string& b,
    size_t weight)
{
  Graph temp(*this);
  temp.addVertex(a);
  temp.addVertex(b);
  EdgeKey key(a, b);
  if (temp.edges_.has(key))
  {
    temp.edges_.at(key).pushBack(weight);
  }
  else
  {
    List< size_t > weights;
    weights.pushBack(weight);
    temp.edges_.add(key, weights);
  }
  swap(temp);
}

bool zhuravleva::Graph::cut(const std::string& a, const std::string& b,
    size_t weight)
{
  if (!hasVertex(a) || !hasVertex(b))
  {
    return false;
  }
  EdgeKey key(a, b);
  if (!edges_.has(key))
  {
    return false;
  }
  Graph temp(*this);
  List< size_t >& weights = temp.edges_.get(key);
  for (auto it = weights.begin(); it != weights.end(); ++it)
  {
    if (*it == weight)
    {
      weights.erase(it);
      if (weights.empty())
      {
        temp.edges_.drop(key);
      }
      swap(temp);
      return true;
    }
  }
  return false;
}

const zhuravleva::myVector< std::string >& zhuravleva::Graph::getVertexes() const
{
  return vertexes_;
}

zhuravleva::EdgeTable& zhuravleva::Graph::getEdges()
{
  return edges_;
}

const zhuravleva::EdgeTable& zhuravleva::Graph::getEdges() const
{
  return edges_;
}

void zhuravleva::Graph::swap(Graph& other) noexcept
{
  vertexes_.swap(other.vertexes_);
  edges_.swap(other.edges_);
}
