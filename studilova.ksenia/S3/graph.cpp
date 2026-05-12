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
