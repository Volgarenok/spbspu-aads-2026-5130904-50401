#include "commands.hpp"

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

  in >> graphName >> count;

  if (!in || graphs.has(graphName))
  {
    out << "<INVALID COMMAND>\n";
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
