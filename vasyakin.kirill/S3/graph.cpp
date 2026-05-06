#include "graph.hpp"

vasyakin::Graph::Graph() noexcept:
  adj(16)
{}

void vasyakin::Graph::addVertex(const std::string& vertex)
{
  if (!adj.has(vertex))
  {
    adj.add(vertex, vasyakin::List< Edge >{});
  }
}
