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

