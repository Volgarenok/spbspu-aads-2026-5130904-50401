#include "commands.hpp"
#include <istream>
#include <ostream>
#include <stdexcept>
#include <utility>

namespace
{
  struct StringComparator
  {
    bool operator()(const std::string& a, const std::string& b) const
    {
      return a < b;
    }
  };

  struct PairComparator
  {
    bool operator()(const std::pair< std::string, size_t >& a,
        const std::pair< std::string, size_t >& b) const
    {
      if (a.first != b.first)
      {
        return a.first < b.first;
      }
      return a.second < b.second;
    }
  };

  template< class T, class Compare >
  void sortVector(zhuravleva::myVector< T >& vector, Compare compare)
  {
    for (size_t i = 0; i < vector.size(); i++)
    {
      size_t minIndex = i;
      for (size_t j = i + 1; j < vector.size(); j++)
      {
        if (compare(vector[j], vector[minIndex]))
        {
          minIndex = j;
        }
      }
      if (minIndex != i)
      {
        T temp = vector[i];
        vector[i] = vector[minIndex];
        vector[minIndex] = temp;
      }
    }
  }

  bool hasString(const zhuravleva::myVector< std::string >& vector, const std::string& value)
  {
    for (size_t i = 0; i < vector.size(); i++)
    {
      if (vector[i] == value)
      {
        return true;
      }
    }
    return false;
  }

  void printGroupedPairs(std::ostream& out, zhuravleva::myVector< std::pair< std::string, size_t > >& result)
  {
    if (result.empty())
    {
      return;
    }
    sortVector(result, PairComparator());
    size_t i = 0;
    while (i < result.size())
    {
      std::string vertex = result[i].first;
      out << vertex;
      while (i < result.size() && result[i].first == vertex)
      {
        out << ' ' << result[i].second;
        i++;
      }
      out << '\n';
    }
  }
}

void zhuravleva::graphs(std::ostream& out, std::istream&, const GraphTable& graphs)
{
  myVector< std::string > names;
  for (GraphTable::ConstIterator it = graphs.cbegin(); it != graphs.cend(); ++it)
  {
    names.pushBack(it->first);
  }
  sortVector(names, StringComparator());
  for (size_t i = 0; i < names.size(); i++)
  {
    out << names[i] << '\n';
  }
}

void zhuravleva::vertexes(std::ostream& out, std::istream& in, const GraphTable& graphs)
{
  std::string graphName;
  if (!(in >> graphName))
  {
    throw std::runtime_error("invalid command");
  }
  if (!graphs.has(graphName))
  {
    throw std::runtime_error("invalid command");
  }
  myVector< std::string > result(graphs.get(graphName).getVertexes());
  sortVector(result, StringComparator());
  for (size_t i = 0; i < result.size(); i++)
  {
    out << result[i] << '\n';
  }
}

void zhuravleva::outbound(std::ostream& out, std::istream& in, const GraphTable& graphs)
{
  std::string graphName;
  std::string vertex;
  if (!(in >> graphName >> vertex))
  {
    throw std::runtime_error("invalid command");
  }
  if (!graphs.has(graphName))
  {
    throw std::runtime_error("invalid command");
  }
  const Graph& graph = graphs.get(graphName);
  if (!graph.hasVertex(vertex))
  {
    throw std::runtime_error("invalid command");
  }
  myVector< std::pair< std::string, size_t > > result;

  const HashTable< std::pair< std::string, std::string >, List< size_t >,
    Blake2Hasher< std::pair< std::string, std::string > >, KeyEqual >& edges = graph.getEdges();

    for (HashTable< std::pair< std::string, std::string >,
      List< size_t >, Blake2Hasher< std::pair< std::string, std::string > >,
      KeyEqual >::ConstIterator it = edges.cbegin(); it != edges.cend(); ++it)
  {
    if (it->first.first == vertex)
    {
      const List< size_t >& weights = it->second;
      for (LCIter< size_t > weightIt = weights.cbegin();
        weightIt != weights.cend(); ++weightIt)
      {
        result.pushBack(std::make_pair(it->first.second, *weightIt));
      }
    }
  }
  printGroupedPairs(out, result);
}

void zhuravleva::inbound(std::ostream& out, std::istream& in, const GraphTable& graphs)
{
  std::string graphName;
  std::string vertex;
  if (!(in >> graphName >> vertex))
  {
    throw std::runtime_error("invalid command");
  }
  if (!graphs.has(graphName))
  {
    throw std::runtime_error("invalid command");
  }
  const Graph& graph = graphs.get(graphName);
  if (!graph.hasVertex(vertex))
  {
    throw std::runtime_error("invalid command");
  }
  myVector< std::pair< std::string, size_t > > result;

  const HashTable< std::pair< std::string, std::string >, List< size_t >,
    Blake2Hasher< std::pair< std::string, std::string > >, KeyEqual >& edges = graph.getEdges();

  for (HashTable< std::pair< std::string, std::string >,
      List< size_t >, Blake2Hasher< std::pair< std::string, std::string > >,
      KeyEqual >::ConstIterator it = edges.cbegin(); it != edges.cend(); ++it)
  {
    if (it->first.second == vertex)
    {
      const List< size_t >& weights = it->second;
      for (LCIter< size_t > weightIt = weights.cbegin();
          weightIt != weights.cend(); ++weightIt)
      {
        result.pushBack(std::make_pair(it->first.first, *weightIt));
      }
    }
  }
  printGroupedPairs(out, result);
}

void zhuravleva::bind(std::ostream&, std::istream& in, GraphTable& graphs)
{
  std::string graphName;
  std::string a;
  std::string b;
  size_t weight = 0;
  if (!(in >> graphName >> a >> b >> weight))
  {
    throw std::runtime_error("invalid command");
  }
  if (!graphs.has(graphName))
  {
    throw std::runtime_error("invalid command");
  }
  graphs.get(graphName).bind(a, b, weight);
}

void zhuravleva::cut(std::ostream&, std::istream& in, GraphTable& graphs)
{
  std::string graphName;
  std::string a;
  std::string b;
  size_t weight = 0;
  if (!(in >> graphName >> a >> b >> weight))
  {
    throw std::runtime_error("invalid command");
  }
  if (!graphs.has(graphName))
  {
    throw std::runtime_error("invalid command");
  }
  if (!graphs.get(graphName).cut(a, b, weight))
  {
    throw std::runtime_error("invalid command");
  }
}

void zhuravleva::create(std::ostream&, std::istream& in, GraphTable& graphs)
{
  std::string graphName;
  size_t count = 0;
  if (!(in >> graphName >> count))
  {
    throw std::runtime_error("invalid command");
  }
  if (graphs.has(graphName))
  {
    throw std::runtime_error("invalid command");
  }
  Graph graph;
  for (size_t i = 0; i < count; i++)
  {
    std::string vertex;
    if (!(in >> vertex))
    {
      throw std::runtime_error("invalid command");
    }
    graph.addVertex(vertex);
  }
  graphs.add(graphName, graph);
}

void zhuravleva::merge(std::ostream&, std::istream& in, GraphTable& graphs)
{
  std::string newGraphName;
  std::string firstGraphName;
  std::string secondGraphName;
  if (!(in >> newGraphName >> firstGraphName >> secondGraphName))
  {
    throw std::runtime_error("invalid command");
  }
  if (graphs.has(newGraphName))
  {
    throw std::runtime_error("invalid command");
  }
  if (!graphs.has(firstGraphName) || !graphs.has(secondGraphName))
  {
    throw std::runtime_error("invalid command");
  }
  const Graph& firstGraph = graphs.get(firstGraphName);
  const Graph& secondGraph = graphs.get(secondGraphName);
  Graph result;
  const myVector< std::string >& firstVertexes = firstGraph.getVertexes();
  for (size_t i = 0; i < firstVertexes.size(); i++)
  {
    result.addVertex(firstVertexes[i]);
  }
  const myVector< std::string >& secondVertexes = secondGraph.getVertexes();
  for (size_t i = 0; i < secondVertexes.size(); i++)
  {
    result.addVertex(secondVertexes[i]);
  }

  const HashTable< std::pair< std::string, std::string >,
    List< size_t >, Blake2Hasher< std::pair< std::string, std::string > >,
    KeyEqual >& firstEdges = firstGraph.getEdges();
  for (HashTable< std::pair< std::string, std::string >,
      List< size_t >, Blake2Hasher< std::pair< std::string, std::string > >,
      KeyEqual >::ConstIterator it = firstEdges.cbegin(); it != firstEdges.cend(); ++it)
  {
    for (LCIter< size_t > weightIt = it->second.cbegin();
        weightIt != it->second.cend(); ++weightIt)
    {
      result.bind(it->first.first, it->first.second, *weightIt);
    }
  }
  const HashTable< std::pair< std::string, std::string >,
    List< size_t >, Blake2Hasher< std::pair< std::string, std::string > >,
    KeyEqual >& secondEdges = secondGraph.getEdges();
  for (HashTable< std::pair< std::string, std::string >,
      List< size_t >, Blake2Hasher< std::pair< std::string, std::string > >,
      KeyEqual >::ConstIterator it = secondEdges.cbegin(); it != secondEdges.cend(); ++it)
  {
    for (LCIter< size_t > weightIt = it->second.cbegin();
        weightIt != it->second.cend(); ++weightIt)
    {
      result.bind(it->first.first, it->first.second, *weightIt);
    }
  }
  graphs.add(newGraphName, result);
}

void zhuravleva::extract(std::ostream&, std::istream& in, GraphTable& graphs)
{
  std::string newGraphName;
  std::string oldGraphName;
  size_t count = 0;
  if (!(in >> newGraphName >> oldGraphName >> count))
  {
    throw std::runtime_error("invalid command");
  }
  if (graphs.has(newGraphName))
  {
    throw std::runtime_error("invalid command");
  }
  if (!graphs.has(oldGraphName))
  {
    throw std::runtime_error("invalid command");
  }
  const Graph& oldGraph = graphs.get(oldGraphName);
  Graph result;
  myVector< std::string > selectedVertexes;
  for (size_t i = 0; i < count; i++)
  {
    std::string vertex;
    if (!(in >> vertex))
    {
      throw std::runtime_error("invalid command");
    }
    if (!oldGraph.hasVertex(vertex))
    {
      throw std::runtime_error("invalid command");
    }
    result.addVertex(vertex);
    selectedVertexes.pushBack(vertex);
  }
  const HashTable< std::pair< std::string, std::string >,
    List< size_t >, Blake2Hasher< std::pair< std::string, std::string > >,
    KeyEqual >& oldEdges = oldGraph.getEdges();
  for (HashTable< std::pair< std::string, std::string >,
      List< size_t >, Blake2Hasher< std::pair< std::string, std::string > >,
      KeyEqual >::ConstIterator it = oldEdges.cbegin(); it != oldEdges.cend(); ++it)
  {
    if (hasString(selectedVertexes, it->first.first) &&
        hasString(selectedVertexes, it->first.second))
    {
      for (LCIter< size_t > weightIt = it->second.cbegin();
          weightIt != it->second.cend();
          ++weightIt)
      {
        result.bind(it->first.first, it->first.second, *weightIt);
      }
    }
  }
  graphs.add(newGraphName, result);
}
