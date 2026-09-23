#include "graph-storage.hpp"
#include <stdexcept>

void yalovsky::GraphStorage::makeGraph(const std::string& name)
{
  if (!isValidName(name))
  {
    throw std::invalid_argument("Invalid graph name");
  }
  if (!graphs_.insert(name, RoadGraph{}))
  {
    throw std::invalid_argument("Graph already exists");
  }
}

void yalovsky::GraphStorage::dropGraph(const std::string& name)
{
  if (!graphs_.erase(name))
  {
    throw std::out_of_range("Graph not found");
  }
}

yalovsky::RoadGraph& yalovsky::GraphStorage::getGraph(const std::string& name)
{
  return graphs_.at(name);
}

const yalovsky::RoadGraph&
yalovsky::GraphStorage::getGraph(const std::string& name) const
{
  return graphs_.at(name);
}

const yalovsky::GraphStorage::Graphs&
yalovsky::GraphStorage::getGraphs() const noexcept
{
  return graphs_;
}

void yalovsky::GraphStorage::swap(GraphStorage& other) noexcept
{
  graphs_.swap(other.graphs_);
}
