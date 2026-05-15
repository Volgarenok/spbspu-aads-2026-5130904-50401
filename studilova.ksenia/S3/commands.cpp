#include "commands.hpp"

#include <utility>

namespace
{
  template< class T, class Cmp >
  void sortVector(studilova::Vector< T >& values, Cmp cmp)
  {
    for (size_t i = 0; i < values.getSize(); ++i)
    {
      for (size_t j = i + 1; j < values.getSize(); ++j)
      {
        if (cmp(values[j], values[i]))
        {
          std::swap(values[i], values[j]);
        }
      }
    }
  }

  bool compareStrings(const std::string& lhs, const std::string& rhs)
  {
    return lhs < rhs;
  }

  bool compareWeights(size_t lhs, size_t rhs)
  {
    return lhs < rhs;
  }

  bool compareConnections(const studilova::Graph::Connection& lhs, const studilova::Graph::Connection& rhs)
  {
    return lhs.first < rhs.first;
  }
}

void studilova::initCommands(CommandsMap& commands)
{
  commands.add("graphs", graphs);
  commands.add("vertexes", vertexes);
  commands.add("outbound", outbound);
  commands.add("inbound", inbound);
  commands.add("bind", bind);
  commands.add("cut", cut);
  commands.add("create", create);
  commands.add("merge", merge);
  commands.add("extract", extract);
}

void studilova::graphs(std::istream&, std::ostream& out, GraphsMap& graphs)
{
  Vector< std::string > names;

  for (auto it = graphs.begin(); it != graphs.end(); ++it)
  {
    auto item = *it;
    names.pushBack(item.first);
  }

  sortVector(names, compareStrings);

  if (names.isEmpty())
  {
    out << "\n";
    return;
  }

  for (size_t i = 0; i < names.getSize(); ++i)
  {
    out << names[i] << "\n";
  }
}

void studilova::vertexes(std::istream& in, std::ostream& out, GraphsMap& graphs)
{
  std::string graphName;
  in >> graphName;

  if (!in || !graphs.has(graphName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  Vector< std::string > vertices = graphs.get(graphName).getVertices();

  if (vertices.isEmpty())
  {
    out << "\n";
    return;
  }

  sortVector(vertices, compareStrings);

  for (size_t i = 0; i < vertices.getSize(); ++i)
  {
    out << vertices[i] << "\n";
  }
}

void studilova::outbound(std::istream& in, std::ostream& out, GraphsMap& graphs)
{
  std::string graphName;
  std::string vertex;

  in >> graphName >> vertex;

  if (!in || !graphs.has(graphName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  try
  {
    Graph::Connections connections = graphs.get(graphName).getOutbound(vertex);

    if (connections.isEmpty())
    {
      out << "\n";
      return;
    }

    sortVector(connections, compareConnections);

    for (size_t i = 0; i < connections.getSize(); ++i)
    {
      Graph::Weights weights = connections[i].second;

      sortVector(weights, compareWeights);

      out << connections[i].first;

      for (size_t j = 0; j < weights.getSize(); ++j)
      {
        out << " " << weights[j];
      }

      out << "\n";
    }
  }
  catch (...)
  {
    out << "<INVALID COMMAND>\n";
  }
}

void studilova::inbound(std::istream& in, std::ostream& out, GraphsMap& graphs)
{
  std::string graphName;
  std::string vertex;

  in >> graphName >> vertex;

  if (!in || !graphs.has(graphName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  try
  {
    Graph::Connections connections = graphs.get(graphName).getInbound(vertex);

    if (connections.isEmpty())
    {
      out << "\n";
      return;
    }

    sortVector(connections, compareConnections);

    for (size_t i = 0; i < connections.getSize(); ++i)
    {
      Graph::Weights weights = connections[i].second;

      sortVector(weights, compareWeights);

      out << connections[i].first;

      for (size_t j = 0; j < weights.getSize(); ++j)
      {
        out << " " << weights[j];
      }

      out << "\n";
    }
  }
  catch (...)
  {
    out << "<INVALID COMMAND>\n";
  }
}

void studilova::bind(std::istream& in, std::ostream& out, GraphsMap& graphs)
{
  std::string graphName;
  std::string from;
  std::string to;
  size_t weight = 0;

  in >> graphName >> from >> to >> weight;

  if (!in || !graphs.has(graphName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  try
  {
    graphs.get(graphName).bind(from, to, weight);
  }
  catch (...)
  {
    out << "<INVALID COMMAND>\n";
  }
}

void studilova::cut(std::istream& in, std::ostream& out, GraphsMap& graphs)
{
  std::string graphName;
  std::string from;
  std::string to;
  size_t weight = 0;

  in >> graphName >> from >> to >> weight;

  if (!in || !graphs.has(graphName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  if (!graphs.get(graphName).cut(from, to, weight))
  {
    out << "<INVALID COMMAND>\n";
  }
}

void studilova::create(std::istream& in, std::ostream& out, GraphsMap& graphs)
{
  std::string graphName;
  size_t count = 0;

  in >> graphName;

  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    in.clear();
    return;
  }

  in >> count;

  if (!in || graphs.has(graphName))
  {
    out << "<INVALID COMMAND>\n";
    in.clear();
    return;
  }

  Graph graph;

  for (size_t i = 0; i < count; ++i)
  {
    std::string vertex;
    in >> vertex;

    if (!in)
    {
      out << "<INVALID COMMAND>\n";
      in.clear();
      return;
    }
    graph.addVertex(vertex);
  }

  try
  {
    graphs.add(graphName, graph);
  }
  catch (...)
  {
    out << "<INVALID COMMAND>\n";
  }
}

void studilova::merge(std::istream& in, std::ostream& out, GraphsMap& graphs)
{
  std::string newGraphName;
  std::string firstGraphName;
  std::string secondGraphName;

  in >> newGraphName >> firstGraphName >> secondGraphName;

  if (
    !in ||
    graphs.has(newGraphName) ||
    !graphs.has(firstGraphName) ||
    !graphs.has(secondGraphName)
  )
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  try
  {
    const Graph& firstGraph = graphs.get(firstGraphName);
    const Graph& secondGraph = graphs.get(secondGraphName);

    Graph newGraph(firstGraph.getEdges().size() + secondGraph.getEdges().size() + 64);

    for (size_t i = 0; i < firstGraph.getVertices().getSize(); ++i)
    {
      newGraph.addVertex(firstGraph.getVertices()[i]);
    }

    for (size_t i = 0; i < secondGraph.getVertices().getSize(); ++i)
    {
      newGraph.addVertex(secondGraph.getVertices()[i]);
    }

    for (auto it = firstGraph.getEdges().begin(); it != firstGraph.getEdges().end(); ++it)
    {
      auto edge = *it;
      const Graph::EdgeKey& key = edge.first;
      const Graph::Weights& weights = edge.second;

      for (size_t i = 0; i < weights.getSize(); ++i)
      {
        newGraph.bind(key.first, key.second, weights[i]);
      }
    }

    for (auto it = secondGraph.getEdges().begin(); it != secondGraph.getEdges().end(); ++it)
    {
      auto edge = *it;
      const Graph::EdgeKey& key = edge.first;
      const Graph::Weights& weights = edge.second;

      for (size_t i = 0; i < weights.getSize(); ++i)
      {
        newGraph.bind(key.first, key.second, weights[i]);
      }
    }

    graphs.add(newGraphName, newGraph);
  }
  catch (...)
  {
    out << "<INVALID COMMAND>\n";
  }
}

void studilova::extract(std::istream& in, std::ostream& out, GraphsMap& graphs)
{
  std::string newGraphName;
  std::string oldGraphName;
  size_t count = 0;

  in >> newGraphName >> oldGraphName >> count;

  if (!in || graphs.has(newGraphName) || !graphs.has(oldGraphName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  try
  {
    const Graph& oldGraph = graphs.get(oldGraphName);

    Graph newGraph(oldGraph.getEdges().size() + 64);

    for (size_t i = 0; i < count; ++i)
    {
      std::string vertex;
      in >> vertex;

      if (!in || !oldGraph.hasVertex(vertex))
      {
        out << "<INVALID COMMAND>\n";
        return;
      }

      newGraph.addVertex(vertex);
    }

    for (auto it = oldGraph.getEdges().begin(); it != oldGraph.getEdges().end(); ++it)
    {
      auto edge = *it;
      const Graph::EdgeKey& key = edge.first;
      const Graph::Weights& weights = edge.second;

      if (newGraph.hasVertex(key.first) && newGraph.hasVertex(key.second))
      {
        for (size_t i = 0; i < weights.getSize(); ++i)
        {
          newGraph.bind(key.first, key.second, weights[i]);
        }
      }
    }

    graphs.add(newGraphName, newGraph);
  }
  catch (...)
  {
    out << "<INVALID COMMAND>\n";
  }
}
