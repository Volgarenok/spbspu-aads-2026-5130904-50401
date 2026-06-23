#include "graph_storage.hpp"

#include <stdexcept>
#include <string>

namespace
{
  bool isEmptyLine(const std::string& line)
  {
    return line.empty() || line.find_first_not_of(" \t\r") == std::string::npos;
  }
}

ulanova::GraphStorage::GraphStorage():
  graphs_(128)
{}

bool ulanova::GraphStorage::hasGraph(const std::string& name) const
{
  return graphs_.contains(name);
}

ulanova::Graph* ulanova::GraphStorage::findGraph(const std::string& name)
{
  auto it = graphs_.find(name);
  if (it == graphs_.end())
  {
    return nullptr;
  }
  return &(*it).second;
}

const ulanova::Graph* ulanova::GraphStorage::findGraph(const std::string& name) const
{
  auto it = graphs_.find(name);
  if (it == graphs_.end())
  {
    return nullptr;
  }
  return &(*it).second;
}

void ulanova::GraphStorage::addGraph(const Graph& graph)
{
  if (graphs_.contains(graph.getName()))
  {
    throw std::invalid_argument("graph already exists");
  }
  graphs_.add(graph.getName(), graph);
}

ulanova::Vector< std::string > ulanova::GraphStorage::getGraphNames() const
{
  Vector< std::string > result;
  for (auto it = graphs_.cbegin(); it != graphs_.cend(); ++it)
  {
    result.push_back((*it).first);
  }
  return result;
}

std::istream& ulanova::loadGraphs(std::istream& input, GraphStorage& storage)
{
  std::string graphName;
  size_t edgeCount = 0;

  while (input >> graphName >> edgeCount)
  {
    Graph graph(graphName);

    for (size_t i = 0; i < edgeCount; ++i)
    {
      std::string from;
      std::string to;
      unsigned weight = 0;

      if (!(input >> from >> to >> weight))
      {
        return input;
      }

      graph.addEdge(from, to, weight);
    }

    storage.addGraph(graph);
  }

  return input;
}
