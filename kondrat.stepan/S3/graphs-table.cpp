#include "graphs-table.hpp"

#include <istream>
#include <ostream>
#include <stdexcept>
#include <utility>
#include "input-utils.hpp"

namespace
{
  using EdgeList = kondrat::Vector< kondrat::EdgeInfo >;

  template< class T >
  void sortVector(kondrat::Vector< T > & values)
  {
    for (size_t i = 0; i < values.getSize(); ++i)
    {
      for (size_t j = i + 1; j < values.getSize(); ++j)
      {
        if (values[j] < values[i])
        {
          std::swap(values[i], values[j]);
        }
      }
    }
  }

  void sortEdges(EdgeList & edges)
  {
    for (size_t i = 0; i < edges.getSize(); ++i)
    {
      for (size_t j = i + 1; j < edges.getSize(); ++j)
      {
        if (edges[j].first < edges[i].first)
        {
          std::swap(edges[i], edges[j]);
        }
      }
    }
  }

  template< class T >
  void printVector(std::ostream & out, const kondrat::Vector< T > & values)
  {
    if (values.isEmpty())
    {
      return;
    }
    out << values[0];
    for (size_t i = 1; i < values.getSize(); ++i)
    {
      out << '\n' << values[i];
    }
  }

  void printEdges(std::ostream & out, EdgeList & edges)
  {
    sortEdges(edges);
    for (size_t i = 0; i < edges.getSize(); ++i)
    {
      if (i != 0)
      {
        out << '\n';
      }
      sortVector(edges[i].second);
      out << edges[i].first;
      for (size_t j = 0; j < edges[i].second.getSize(); ++j)
      {
        out << ' ' << edges[i].second[j];
      }
    }
  }
}

void kondrat::GraphsTable::readFile(std::istream & in)
{
  std::string graphName;
  size_t edgesCount = 0;
  while (in >> graphName >> edgesCount)
  {
    Graph graph;
    for (size_t i = 0; i < edgesCount; ++i)
    {
      std::string from;
      std::string to;
      size_t weight = 0;
      if (!(in >> from >> to >> weight))
      {
        throw std::logic_error("invalid file");
      }
      graph.bind(from, to, weight);
    }
    graphs_.add(graphName, std::move(graph));
  }
}

bool kondrat::GraphsTable::graphs(std::istream &, std::ostream & out, const std::string &)
{
  Vector< std::string > names;
  HTIter< std::string, Graph > it = graphs_.begin();
  const HTIter< std::string, Graph > end = graphs_.end();
  while (it != end)
  {
    names.pushBack(it->first);
    ++it;
  }
  sortVector(names);
  printVector(out, names);
  return true;
}

bool kondrat::GraphsTable::vertexes(std::istream &, std::ostream & out, const std::string & graphName)
{
  Graph & graph = graphs_.at(graphName);
  Vector< std::string > vertexes = graph.getVertexes();
  sortVector(vertexes);
  printVector(out, vertexes);
  return true;
}

bool kondrat::GraphsTable::outbound(std::istream & in, std::ostream & out, const std::string & graphName)
{
  std::string vertex;
  if (!detail::readArgument(in, vertex))
  {
    throw std::logic_error("invalid command");
  }
  Graph & graph = graphs_.at(graphName);
  EdgeList edges = graph.getOutbound(vertex);
  printEdges(out, edges);
  return true;
}

bool kondrat::GraphsTable::inbound(std::istream & in, std::ostream & out, const std::string & graphName)
{
  std::string vertex;
  if (!detail::readArgument(in, vertex))
  {
    throw std::logic_error("invalid command");
  }
  Graph & graph = graphs_.at(graphName);
  EdgeList edges = graph.getInbound(vertex);
  printEdges(out, edges);
  return true;
}

bool kondrat::GraphsTable::bind(std::istream & in, std::ostream &, const std::string & graphName)
{
  std::string from;
  std::string to;
  size_t weight = 0;
  if (!detail::readArgument(in, from)
      || !detail::readArgument(in, to)
      || !detail::readArgument(in, weight))
  {
    throw std::logic_error("invalid command");
  }
  graphs_.at(graphName).bind(from, to, weight);
  return false;
}

bool kondrat::GraphsTable::cut(std::istream & in, std::ostream &, const std::string & graphName)
{
  std::string from;
  std::string to;
  size_t weight = 0;
  if (!detail::readArgument(in, from)
      || !detail::readArgument(in, to)
      || !detail::readArgument(in, weight))
  {
    throw std::logic_error("invalid command");
  }
  graphs_.at(graphName).cut(from, to, weight);
  return false;
}

bool kondrat::GraphsTable::create(std::istream & in, std::ostream &, const std::string & graphName)
{
  size_t count = 0;
  if (!detail::readArgument(in, count) || graphs_.contains(graphName))
  {
    throw std::logic_error("invalid command");
  }

  Graph graph;
  for (size_t i = 0; i < count; ++i)
  {
    std::string vertex;
    if (!detail::readArgument(in, vertex))
    {
      throw std::logic_error("invalid command");
    }
    graph.addVertex(vertex);
  }

  GraphStorage copy(graphs_);
  copy.add(graphName, std::move(graph));
  graphs_.swap(copy);
  return false;
}

bool kondrat::GraphsTable::merge(std::istream & in, std::ostream &, const std::string & graphName)
{
  std::string firstName;
  std::string secondName;
  if (!detail::readArgument(in, firstName)
      || !detail::readArgument(in, secondName))
  {
    throw std::logic_error("invalid command");
  }
  const bool invalidNames = graphs_.contains(graphName)
    || !graphs_.contains(firstName)
    || !graphs_.contains(secondName);
  if (invalidNames)
  {
    throw std::logic_error("invalid command");
  }

  Graph graph;
  graph.mergeFrom(graphs_.at(firstName), graphs_.at(secondName));
  GraphStorage copy(graphs_);
  copy.add(graphName, std::move(graph));
  graphs_.swap(copy);
  return false;
}

bool kondrat::GraphsTable::extract(std::istream & in, std::ostream &, const std::string & graphName)
{
  std::string oldName;
  size_t count = 0;
  if (!detail::readArgument(in, oldName)
      || !detail::readArgument(in, count))
  {
    throw std::logic_error("invalid command");
  }
  if (graphs_.contains(graphName) || !graphs_.contains(oldName))
  {
    throw std::logic_error("invalid command");
  }

  Vector< std::string > vertexes;
  for (size_t i = 0; i < count; ++i)
  {
    std::string vertex;
    if (!detail::readArgument(in, vertex))
    {
      throw std::logic_error("invalid command");
    }
    vertexes.pushBack(vertex);
  }

  Graph graph;
  graph.extractFrom(graphs_.at(oldName), vertexes);
  GraphStorage copy(graphs_);
  copy.add(graphName, std::move(graph));
  graphs_.swap(copy);
  return false;
}
