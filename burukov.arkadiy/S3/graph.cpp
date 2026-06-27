#include "graph.hpp"

#include <utility>
#include <stdexcept>

namespace
{
  bool listContains(const burukov::Graph::VertexList &list, const std::string &value)
  {
    for (auto it = list.cbegin(); it != list.cend(); ++it)
    {
      if (*it == value)
      {
        return true;
      }
    }
    return false;
  }
}

burukov::Graph::Graph(size_t bucketHint):
  edges_(bucketHint)
{}

bool burukov::Graph::hasVertex(const std::string &vertexName) const
{
  return listContains(vertices_, vertexName);
}

const burukov::Graph::VertexList &burukov::Graph::vertices() const noexcept
{
  return vertices_;
}

const burukov::Graph::EdgeTable &burukov::Graph::edges() const noexcept
{
  return edges_;
}

void burukov::Graph::addVertex(const std::string &vertexName)
{
  if (hasVertex(vertexName))
  {
    return;
  }
  vertices_.pushFront(vertexName);
}

void burukov::Graph::addEdge(const std::string &from, const std::string &to, size_t weight)
{
  VertexList verticesCopy(vertices_);
  EdgeTable edgesCopy(edges_);
  if (!listContains(verticesCopy, from))
  {
    verticesCopy.pushFront(from);
  }
  if (to != from && !listContains(verticesCopy, to))
  {
    verticesCopy.pushFront(to);
  }
  const EdgeKey key(from, to);
  if (edgesCopy.contains(key))
  {
    edgesCopy.at(key).pushFront(weight);
  }
  else
  {
    WeightList list;
    list.pushFront(weight);
    edgesCopy.add(key, list);
  }
  vertices_.swap(verticesCopy);
  edges_.swap(edgesCopy);
}

void burukov::Graph::removeEdge(const std::string &from, const std::string &to, size_t weight)
{
  const EdgeKey key(from, to);
  if (!edges_.contains(key))
  {
    throw std::out_of_range("edge not found");
  }
  EdgeTable edgesCopy(edges_);
  WeightList &list = edgesCopy.at(key);
  WeightList rebuilt;
  bool removed = false;
  for (auto it = list.cbegin(); it != list.cend(); ++it)
  {
    if (!removed && *it == weight)
    {
      removed = true;
      continue;
    }
    rebuilt.pushBack(*it);
  }
  if (!removed)
  {
    throw std::out_of_range("weight not found");
  }
  if (rebuilt.empty())
  {
    edgesCopy.erase(key);
  }
  else
  {
    list = std::move(rebuilt);
  }
  edges_.swap(edgesCopy);
}
