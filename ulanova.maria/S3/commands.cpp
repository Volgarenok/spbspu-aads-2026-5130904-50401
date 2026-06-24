#include "commands.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>

void ulanova::sortStrings(Vector< std::string >& values)
{
  std::sort(values.begin(), values.end());
}

bool edgeLess(const ulanova::Edge& lhs, const ulanova::Edge& rhs)
{
  if (lhs.vertex != rhs.vertex)
  {
    return lhs.vertex < rhs.vertex;
  }
  return lhs.weight < rhs.weight;
}

void ulanova::sortEdges(Vector< Edge >& edges)
{
  std::sort(edges.begin(), edges.end(), edgeLess);
}

void ulanova::printEdges(const Vector< Edge >& edges, std::ostream& output)
{
  if (edges.isEmpty())
  {
    output << '\n';
    return;
  }

  size_t i = 0;
  while (i < edges.getsize())
  {
    output << edges[i].vertex << ' ' << edges[i].weight;

    size_t j = i + 1;
    while (j < edges.getsize() && edges[j].vertex == edges[i].vertex)
    {
      output << ' ' << edges[j].weight;
      ++j;
    }

    output << '\n';
    i = j;
  }
}

void ulanova::printInvalid(std::ostream& output)
{
  output << "<INVALID COMMAND>\n";
}

ulanova::CommandProcessor::CommandProcessor(GraphStorage& storage):
  storage_(storage),
  handlers_()
{
  handlers_.add("graphs",   &CommandProcessor::handleGraphs);
  handlers_.add("vertexes", &CommandProcessor::handleVertices);
  handlers_.add("vertices", &CommandProcessor::handleVertices);
  handlers_.add("outbound", &CommandProcessor::handleOutbound);
  handlers_.add("inbound",  &CommandProcessor::handleInbound);
  handlers_.add("bind",     &CommandProcessor::handleBind);
  handlers_.add("cut",      &CommandProcessor::handleCut);
  handlers_.add("create",   &CommandProcessor::handleCreate);
  handlers_.add("merge",    &CommandProcessor::handleMerge);
  handlers_.add("extract",  &CommandProcessor::handleExtract);
}

void ulanova::CommandProcessor::run(std::istream& input, std::ostream& output)
{
  std::string cmd;
  while (input >> cmd)
  {
    Handler* it = handlers_.find(cmd);
    if (it == nullptr)
    {
      std::string rest;
      std::getline(input, rest);
      printInvalid(output);
      continue;
    }
    try
    {
      Handler handler = *it;
      (this->*handler)(input, output);
    }
    catch (...)
    {
      printInvalid(output);
    }
  }
}

void ulanova::CommandProcessor::handleGraphs(std::istream&, std::ostream& output)
{
  Vector< std::string > names = storage_.getGraphNames();
  sortStrings(names);
  if (names.isEmpty())
  {
    output << '\n';
    return;
  }
  for (auto it = names.begin(); it != names.end(); ++it)
  {
    output << *it << '\n';
  }
}

void ulanova::CommandProcessor::handleVertices(std::istream& input, std::ostream& output)
{
  std::string graphName;
  if (!(input >> graphName))
  {
    throw std::runtime_error("bad vertices");
  }
  const Graph* graph = storage_.findGraph(graphName);
  if (graph == nullptr)
  {
    throw std::runtime_error("graph not found");
  }
  Vector< std::string > vertices = graph->getVertices();
  sortStrings(vertices);
  if (vertices.isEmpty())
  {
    output << '\n';
    return;
  }
  for (auto it = vertices.begin(); it != vertices.end(); ++it)
  {
    output << *it << '\n';
  }
}

void ulanova::CommandProcessor::handleOutbound(std::istream& input, std::ostream& output)
{
  std::string graphName;
  std::string vertex;
  if (!(input >> graphName >> vertex))
  {
    throw std::runtime_error("bad outbound");
  }
  const Graph* graph = storage_.findGraph(graphName);
  if (graph == nullptr || !graph->hasVertex(vertex))
  {
    throw std::runtime_error("graph or vertex not found");
  }
  Vector< Edge > edges = graph->getOutbound(vertex);
  sortEdges(edges);
  printEdges(edges, output);
}

void ulanova::CommandProcessor::handleInbound(std::istream& input, std::ostream& output)
{
  std::string graphName;
  std::string vertex;
  if (!(input >> graphName >> vertex))
  {
    throw std::runtime_error("bad inbound");
  }
  const Graph* graph = storage_.findGraph(graphName);
  if (graph == nullptr || !graph->hasVertex(vertex))
  {
    throw std::runtime_error("graph or vertex not found");
  }
  Vector< Edge > edges = graph->getInbound(vertex);
  sortEdges(edges);
  printEdges(edges, output);
}

void ulanova::CommandProcessor::handleBind(std::istream& input, std::ostream&)
{
  std::string graphName;
  std::string from;
  std::string to;
  unsigned weight = 0;
  if (!(input >> graphName >> from >> to >> weight))
  {
    throw std::runtime_error("bad bind");
  }
  Graph* graph = storage_.findGraph(graphName);
  if (graph == nullptr)
  {
    throw std::runtime_error("graph not found");
  }
  graph->addEdge(from, to, weight);
}

void ulanova::CommandProcessor::handleCut(std::istream& input, std::ostream& output)
{
  std::string graphName;
  std::string from;
  std::string to;
  unsigned weight = 0;
  if (!(input >> graphName >> from >> to >> weight))
  {
    throw std::runtime_error("bad cut");
  }
  Graph* graph = storage_.findGraph(graphName);
  if (graph == nullptr || !graph->removeEdge(from, to, weight))
  {
    printInvalid(output);
  }
}

void ulanova::CommandProcessor::handleCreate(std::istream& input, std::ostream& output)
{
  std::string graphName;
  size_t count = 0;
  if (!(input >> graphName >> count))
  {
    throw std::runtime_error("bad create");
  }
  if (storage_.hasGraph(graphName))
  {
    std::string rest;
    std::getline(input, rest);
    printInvalid(output);
    return;
  }
  Graph graph(graphName);
  for (size_t i = 0; i < count; ++i)
  {
    std::string vertex;
    if (!(input >> vertex))
    {
      throw std::runtime_error("bad create: not enough vertices");
    }
    graph.addVertex(vertex);
  }
  storage_.addGraph(graph);
}

void ulanova::CommandProcessor::handleMerge(std::istream& input, std::ostream& output)
{
  std::string newName;
  std::string lhsName;
  std::string rhsName;
  if (!(input >> newName >> lhsName >> rhsName))
  {
    throw std::runtime_error("bad merge");
  }
  if (storage_.hasGraph(newName))
  {
    printInvalid(output);
    return;
  }
  const Graph* lhs = storage_.findGraph(lhsName);
  const Graph* rhs = storage_.findGraph(rhsName);
  if (lhs == nullptr || rhs == nullptr)
  {
    throw std::runtime_error("graph not found");
  }
  Graph result = Graph::merge(newName, *lhs, *rhs);
  storage_.addGraph(result);
}

void ulanova::CommandProcessor::handleExtract(std::istream& input, std::ostream& output)
{
  std::string newName;
  std::string srcName;
  size_t count = 0;
  if (!(input >> newName >> srcName >> count))
  {
    throw std::runtime_error("bad extract");
  }
  if (storage_.hasGraph(newName))
  {
    printInvalid(output);
    return;
  }
  const Graph* graph = storage_.findGraph(srcName);
  if (graph == nullptr)
  {
    throw std::runtime_error("graph not found");
  }
  Vector< std::string > vertices;
  for (size_t i = 0; i < count; ++i)
  {
    std::string vertex;
    if (!(input >> vertex))
    {
      throw std::runtime_error("bad extract: not enough vertices");
    }
    vertices.push_back(vertex);
  }
  Graph result = graph->extract(newName, vertices);
  storage_.addGraph(result);
}
