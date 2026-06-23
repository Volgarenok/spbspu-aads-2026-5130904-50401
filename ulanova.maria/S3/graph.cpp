#include "graph.hpp"

ulanova::EdgeKey::EdgeKey():
  from(),
  to(),
  weight(0)
{}

ulanova::EdgeKey::EdgeKey(const std::string& edgeFrom, const std::string& edgeTo, unsigned edgeWeight):
  from(edgeFrom),
  to(edgeTo),
  weight(edgeWeight)
{}

bool ulanova::EdgeKey::operator==(const EdgeKey& rhs) const
{
  return from == rhs.from && to == rhs.to && weight == rhs.weight;
}

ulanova::Edge::Edge():
  vertex(),
  weight(0)
{}

ulanova::Edge::Edge(const std::string& vertexName, unsigned edgeWeight):
  vertex(vertexName),
  weight(edgeWeight)
{}

bool ulanova::Edge::operator==(const Edge& rhs) const
{
  return vertex == rhs.vertex && weight == rhs.weight;
}

ulanova::Graph::Graph():
  name_(),
  vertices_(),
  outbound_(),
  inbound_()
{}

ulanova::Graph::Graph(const std::string& name):
  name_(name),
  vertices_(),
  outbound_(),
  inbound_()
{}

const std::string& ulanova::Graph::getName() const noexcept
{
  return name_;
}

bool ulanova::Graph::hasVertex(const std::string& vertex) const
{
  return vertices_.contains(vertex);
}

void ulanova::Graph::addVertex(const std::string& vertex)
{
  if (!vertices_.contains(vertex))
  {
    vertices_.add(vertex, true);
    outbound_.add(vertex, Vector< Edge >());
    inbound_.add(vertex, Vector< Edge >());
  }
}

bool ulanova::Graph::hasEdge(const std::string& from, const std::string& to, unsigned weight) const
{
  auto it = outbound_.find(from);
  if (it == outbound_.end())
  {
    return false;
  }
  const Vector< Edge >& edges = (*it).second;
  for (auto edgeIt = edges.begin(); edgeIt != edges.end(); ++edgeIt)
  {
    if (edgeIt->vertex == to && edgeIt->weight == weight)
    {
      return true;
    }
  }
  return false;
}

void ulanova::Graph::addEdge(const std::string& from, const std::string& to, unsigned weight)
{
  bool fromNew = !vertices_.contains(from);
  bool toNew = !vertices_.contains(to);

  addVertex(from);

  try
  {
    addVertex(to);
  }
  catch (...)
  {
    if (fromNew)
    {
      vertices_.erase(from);
      outbound_.erase(from);
      inbound_.erase(from);
    }
    throw;
  }

  auto outIt = outbound_.find(from);
  auto inIt = inbound_.find(to);

  (*outIt).second.push_back(Edge(to, weight));

  try
  {
    (*inIt).second.push_back(Edge(from, weight));
  }
  catch (...)
  {
    (*outIt).second.popback();
    if (toNew)
    {
      vertices_.erase(to);
      outbound_.erase(to);
      inbound_.erase(to);
    }
    if (fromNew)
    {
      vertices_.erase(from);
      outbound_.erase(from);
      inbound_.erase(from);
    }
    throw;
  }
}

bool ulanova::Graph::removeEdge(const std::string& from, const std::string& to, unsigned weight)
{
  auto outIt = outbound_.find(from);
  auto inIt = inbound_.find(to);

  if (outIt == outbound_.end() || inIt == inbound_.end())
  {
    return false;
  }

  Vector< Edge >& outEdges = (*outIt).second;
  Vector< Edge >& inEdges = (*inIt).second;

  bool removedOut = false;
  for (size_t i = 0; i < outEdges.getsize(); ++i)
  {
    if (outEdges[i].vertex == to && outEdges[i].weight == weight)
    {
      outEdges.erase(i);
      removedOut = true;
      break;
    }
  }

  if (!removedOut)
  {
    return false;
  }

  for (size_t i = 0; i < inEdges.getsize(); ++i)
  {
    if (inEdges[i].vertex == from && inEdges[i].weight == weight)
    {
      inEdges.erase(i);
      return true;
    }
  }

  return true;
}

ulanova::Vector< std::string > ulanova::Graph::getVertices() const
{
  Vector< std::string > result;
  for (auto it = vertices_.cbegin(); it != vertices_.cend(); ++it)
  {
    result.push_back((*it).first);
  }
  return result;
}

ulanova::Vector< ulanova::Edge > ulanova::Graph::getOutbound(const std::string& vertex) const
{
  auto it = outbound_.find(vertex);
  if (it == outbound_.end())
  {
    return Vector< Edge >();
  }
  return (*it).second;
}

ulanova::Vector< ulanova::Edge > ulanova::Graph::getInbound(const std::string& vertex) const
{
  auto it = inbound_.find(vertex);
  if (it == inbound_.end())
  {
    return Vector< Edge >();
  }
  return (*it).second;
}

ulanova::Graph ulanova::Graph::merge(const std::string& name, const Graph& rhs) const
{
  Graph result(name);

  Vector< std::string > lhsVertices = getVertices();
  for (auto it = lhsVertices.begin(); it != lhsVertices.end(); ++it)
  {
    result.addVertex(*it);
  }

  Vector< std::string > rhsVertices = rhs.getVertices();
  for (auto it = rhsVertices.begin(); it != rhsVertices.end(); ++it)
  {
    result.addVertex(*it);
  }

  for (auto it = outbound_.cbegin(); it != outbound_.cend(); ++it)
  {
    const std::string& from = (*it).first;
    const Vector< Edge >& edges = (*it).second;
    for (auto edgeIt = edges.begin(); edgeIt != edges.end(); ++edgeIt)
    {
      result.addEdge(from, edgeIt->vertex, edgeIt->weight);
    }
  }

  for (auto it = rhs.outbound_.cbegin(); it != rhs.outbound_.cend(); ++it)
  {
    const std::string& from = (*it).first;
    const Vector< Edge >& edges = (*it).second;
    for (auto edgeIt = edges.begin(); edgeIt != edges.end(); ++edgeIt)
    {
      result.addEdge(from, edgeIt->vertex, edgeIt->weight);
    }
  }

  return result;
}

ulanova::Graph ulanova::Graph::extract(const std::string& name, const Vector< std::string >& vertices) const
{
  Graph result(name);

  for (auto it = vertices.begin(); it != vertices.end(); ++it)
  {
    if (!hasVertex(*it))
    {
      throw std::out_of_range("vertex not found");
    }
    result.addVertex(*it);
  }

  for (auto it = vertices.begin(); it != vertices.end(); ++it)
  {
    const std::string& from = *it;
    auto edgeIt = outbound_.find(from);
    if (edgeIt == outbound_.end())
    {
      continue;
    }
    const Vector< Edge >& edges = (*edgeIt).second;
    for (auto eIt = edges.begin(); eIt != edges.end(); ++eIt)
    {
      if (result.hasVertex(eIt->vertex))
      {
        result.addEdge(from, eIt->vertex, eIt->weight);
      }
    }
  }

  return result;
}
