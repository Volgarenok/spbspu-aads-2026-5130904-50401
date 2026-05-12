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
