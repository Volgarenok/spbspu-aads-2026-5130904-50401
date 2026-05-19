#include "graph.hpp"

zhuravleva::Graph::Graph(size_t bucket_count):
  vertexes(),
  edges(bucket_count)
{}

bool zhuravleva::Graph::hasVertex(const std::string& vertex) const
{
  for (auto it = vertexes.cbegin(); it != vertexes.cend(); ++it)
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
    vertexes.pushBack(vertex);
  }
}

void zhuravleva::Graph::bind(const std::string& a, const std::string& b,
    size_t weight)
{
  addVertex(a);
  addVertex(b);

  std::pair< std::string, std::string > key(a, b);

  if (edges.has(key))
  {
    edges.get(key).addEnd(weight);
  }
  else
  {
    List< size_t > weights;
    weights.addEnd(weight);
    edges.add(key, weights);
  }
}

bool zhuravleva::Graph::cut(const std::string& a, const std::string& b, size_t weight)
{
  if (!hasVertex(a) || !hasVertex(b))
  {
    return false;
  }
  std::pair< std::string, std::string > key(a, b);
  if (!edges.has(key))
  {
    return false;
  }
  List< size_t >& weights = edges.get(key);
  for (auto it = weights.begin(); it != weights.end(); ++it)
  {
    if (*it == weight)
    {
      weights.erase(it);
      if (weights.empty())
      {
        edges.drop(key);
      }
      return true;
    }
  }
  return false;
}

const zhuravleva::myVector< std::string >& zhuravleva::Graph::getVertexes() const
{
  return vertexes;
}

zhuravleva::HashTable<
  std::pair< std::string, std::string >,
  zhuravleva::List< size_t >,
  zhuravleva::Blake2Hasher< std::pair< std::string, std::string > >,
  zhuravleva::KeyEqual >& zhuravleva::Graph::getEdges()
{
  return edges;
}

const zhuravleva::HashTable<
  std::pair< std::string, std::string >,
  zhuravleva::List< size_t >,
  zhuravleva::Blake2Hasher< std::pair< std::string, std::string > >,
  zhuravleva::KeyEqual >& zhuravleva::Graph::getEdges() const
{
  return edges;
}
