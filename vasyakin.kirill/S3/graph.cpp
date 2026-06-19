#include "graph.hpp"

namespace
{
  struct EdgePredicate
  {
    const std::string& to_;
    size_t weight_;

    bool operator()(const vasyakin::Edge& e) const
    {
      return e.to == to_ && e.weight == weight_;
    }
  };
}


vasyakin::Graph::Graph() noexcept:
  adj(16)
{}

void vasyakin::Graph::addVertex(const std::string& vertex)
{
  if (!adj.contains(vertex))
  {
    adj.insert(std::make_pair(vertex, vasyakin::List< Edge >{}));
  }
}

void vasyakin::Graph::addEdge(const std::string& from, const std::string& to, size_t weight)
{
  if (adj.contains(from))
  {
    adj.at(from).pushBack({to, weight});
  }
  else
  {
    List< Edge > edges;
    edges.pushBack({to, weight});
    adj.insert(std::make_pair(from, std::move(edges)));
  }
}

bool vasyakin::Graph::hasVertex(const std::string& vertex) const
{
  return adj.contains(vertex);
}

bool vasyakin::Graph::hasEdge(const std::string& from, const std::string& to, size_t weight) const
{
  if (!adj.contains(from))
  {
    return false;
  }

  const auto& edges = adj.at(from);
  for (auto it = edges.begin(); it != edges.end(); ++it)
  {
    if (it->to == to && it->weight == weight)
    {
      return true;
    }
  }

  return false;
}

void vasyakin::Graph::removeEdge(const std::string& from, const std::string& to, size_t weight)
{
  if (!adj.contains(from))
  {
    return;
  }

  auto& edges = adj.at(from);

  edges.erase_if(EdgePredicate{to, weight});
}

vasyakin::Vector< std::string > vasyakin::Graph::getAllVertices() const
{
  vasyakin::Vector< std::string > vertices;
  for (auto it = adj.begin(); it != adj.end(); ++it)
  {
    vertices.push_back(it->first);
  }

  return vertices;
}

vasyakin::Vector< std::pair< std::string, size_t > > vasyakin::Graph::getOutgoingEdges(
  const std::string& vertex) const
{
  vasyakin::Vector< std::pair< std::string, size_t > > edges;
  if (!adj.contains(vertex))
  {
    return edges;
  }

  const auto& vertexEdges = adj.at(vertex);
  for (auto it = vertexEdges.begin(); it != vertexEdges.end(); ++it)
  {
    edges.push_back({it->to, it->weight});
  }

  return edges;
}

vasyakin::Vector< std::pair< std::string, size_t > > vasyakin::Graph::getIncomingEdges(
  const std::string& vertex) const
{
  vasyakin::Vector< std::pair< std::string, size_t > > edges;
  for (auto it = adj.begin(); it != adj.end(); ++it)
  {
    const auto& fromVertex = it->first;
    const auto& vertexEdges = it->second;

    for (auto eit = vertexEdges.begin(); eit != vertexEdges.end(); ++eit)
    {
      if (eit->to == vertex)
      {
        edges.push_back({fromVertex, eit->weight});
      }
    }
  }

  return edges;
}
