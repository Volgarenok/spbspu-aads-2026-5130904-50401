#include "graph.hpp"

sedov::Graph::Graph() noexcept:
  vertices(),
  edges(16)
{}

sedov::Graph::Graph(size_t slots):
  vertices(),
  edges(slots)
{}

void sedov::Graph::addVertex(const std::string & v)
{
  bool exists = false;
  for (LIter< std::string > it = vertices.begin(); it != vertices.end(); ++it)
  {
    if (*it == v)
    {
      exists = true;
      break;
    }
  }
  if (!exists)
  {
    vertices.pushBack(v);
  }
}

void sedov::Graph::removeVertex(const std::string & v)
{
  bool found = false;
  for (LIter< std::string > it = vertices.begin(); it != vertices.end(); ++it)
  {
    if (*it == v)
    {
      vertices.erase(it);
      found = true;
      break;
    }
  }
  if (!found)
  {
    throw std::out_of_range("Vertex not found");
  }

  sedov::Vector< keyPair > to_remove;
  for (auto it = edges.begin(); it != edges.end(); ++it)
  {
    if ((*it).first.first == v || (*it).first.second == v)
    {
      to_remove.pushBack((*it).first);
    }
  }
  for (size_t i = 0; i < to_remove.getSize(); ++i)
  {
    edges.erase(to_remove[i]);
  }
}

void sedov::Graph::addEdge(const std::string & from, const std::string & to, size_t weight)
{
  sedov::List< std::string > tempVertices = vertices;
  bool fromExists = false;
  bool toExists = false;
  for (LIter< std::string > it = tempVertices.begin(); it != tempVertices.end(); ++it)
  {
    if (*it == from)
    {
      fromExists = true;
    }
    if (*it == to)
    {
      toExists = true;
    }
  }
  if (!fromExists)
  {
    tempVertices.pushBack(from);
  }
  if (!toExists)
  {
    tempVertices.pushBack(to);
  }
  auto tempEdges = edges;
  keyPair key(from, to);
  if (tempEdges.contains(key))
  {
    tempEdges.at(key).pushBack(weight);
  }
  else
  {
    listWeights weights;
    weights.pushBack(weight);
    tempEdges.add(key, weights);
  }
  vertices.swap(tempVertices);
  edges.swap(tempEdges);
}

void sedov::Graph::removeEdge(const std::string & from, const std::string & to, size_t weight)
{
  keyPair key(from, to);
  if (!edges.contains(key))
  {
    throw std::out_of_range("Edge not found");
  }
  listWeights & weights = edges.at(key);
  bool found = false;
  LIter< size_t > it = weights.begin();
  while (it != weights.end())
  {
    if (*it == weight)
    {
      it = weights.erase(it);
      found = true;
      break;
    }
    else
    {
      ++it;
    }
  }
  if (!found)
  {
    throw std::out_of_range("Weight not found");
  }
  if (weights.size() == 0)
  {
    edges.erase(key);
  }
}
