#include "graph_storage.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace ulanova
{
  GraphStorage::GraphStorage():
    graphs_(128)
  {}

  bool GraphStorage::hasGraph(const std::string& name) const
  {
    return graphs_.has(name);
  }

  Graph* GraphStorage::findGraph(const std::string& name)
  {
    return graphs_.find(name);
  }

  const Graph* GraphStorage::findGraph(const std::string& name) const
  {
    return graphs_.find(name);
  }

  void GraphStorage::addGraph(const Graph& graph)
  {
    if (graphs_.has(graph.getName()))
    {
      throw std::invalid_argument("graph already exists");
    }

    graphs_.add(graph.getName(), graph);
  }

  Vector<std::string> GraphStorage::getGraphNames() const
  {
    Vector<std::string> result;

    for (auto it = graphs_.cbegin(); it != graphs_.cend(); ++it)
    {
      result.push_back(it.key());
    }

    return result;
  }

  static bool isEmptyLine(const std::string& line)
  {
    for (size_t i = 0; i < line.size(); ++i)
    {
      if (line[i] != ' ' && line[i] != '\t' && line[i] != '\r')
      {
        return false;
      }
    }

    return true;
  }

  void loadGraphsFromFile(const std::string& filename, GraphStorage& storage)
  {
    std::ifstream input(filename);

    if (!input)
    {
      throw std::runtime_error("cannot open input file");
    }

    std::string line;

    while (std::getline(input, line))
    {
      if (isEmptyLine(line))
      {
        continue;
      }

      std::istringstream header(line);
      std::string graphName;
      size_t edgeCount = 0;

      if (!(header >> graphName >> edgeCount))
      {
        throw std::runtime_error("bad graph header");
      }

      Graph graph(graphName);

      size_t readEdges = 0;
      while (readEdges < edgeCount && std::getline(input, line))
      {
        if (isEmptyLine(line))
        {
          continue;
        }

        std::istringstream edgeLine(line);
        std::string from;
        std::string to;
        unsigned weight = 0;

        if (!(edgeLine >> from >> to >> weight))
        {
          throw std::runtime_error("bad edge line");
        }

        graph.addEdge(from, to, weight);
        ++readEdges;
      }

      if (readEdges != edgeCount)
      {
        throw std::runtime_error("not enough edge lines");
      }

      storage.addGraph(graph);
    }
  }
}
