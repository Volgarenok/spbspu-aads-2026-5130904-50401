#include "graph.hpp"

void burukov::Graph::addVertex(const std::string &vertexName)
{
  for (auto it = vertices_.begin(); it != vertices_.end(); ++it)
  {
    if (*it == vertexName)
    {
      return;
    }
  }
  vertices_.pushFront(vertexName);
}

void burukov::Graph::addEdge(const std::string &from, const std::string &to, size_t weight)
{
  addVertex(from);
  addVertex(to);
  EdgeKey key(from, to);
  if (edges_.contains(key))
  {
    edges_.at(key).pushFront(weight);
  }
  else
  {
    WeightList list;
    list.pushFront(weight);
    edges_.add(key, list);
  }
}

void burukov::Graph::removeEdge(const std::string &from, const std::string &to, size_t weight)
{
  EdgeKey key(from, to);
  if (!edges_.contains(key))
  {
    throw std::out_of_range("edge not found");
  }
  WeightList &list = edges_.at(key);
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
    edges_.erase(key);
  }
  else
  {
    list = std::move(rebuilt);
  }
}
