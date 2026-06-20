#include "graphs_table.hpp"
void malashenko::GraphsTable::graphs(std::istream&, std::ostream& out, std::string)
{
  Vector< std::string > tmpTops;
  for (htIter_t start = graphs_.begin(); start != graphs_.end(); ++start)
  {
    tmpTops.pushBack(start->first);
  }
  if (tmpTops.isEmpty())
  {
    return;
  }
  Vector< std::string > tops = rmRepeatStrs(tmpTops);
  sortStrs(tops);
  out << tops[0];
  for (size_t i = 1; i < tops.getSize(); ++i)
  {
    out << '\n' << tops[i];
  }
}
void malashenko::GraphsTable::vertexes(std::istream&, std::ostream& out, std::string graphName)
{
  if (!graphs_.contains(graphName))
  {
    throw std::invalid_argument("unkown graph");
  }
  Graph& graph = graphs_.at(graphName);
  Vector< std::string > tmpTops = graph.getTops();
  if (tmpTops.isEmpty())
  {
    return;
  }
  Vector< std::string > tops = rmRepeatStrs(tmpTops);
  sortStrs(tops);
  out << tops[0];
  for (size_t i = 1; i < tops.getSize(); ++i)
  {
    out << '\n' << tops[i];
  }
}
std::istream& malashenko::GraphsTable::bounds(std::istream& in, std::ostream& out, std::string graphName, size_t pos)
{
  if (!graphs_.contains(graphName))
  {
    throw std::invalid_argument("unkown graph");
  }
  Graph& graph = graphs_.at(graphName);
  std::string topName;
  if (!(in >> topName))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::pair< bool, Vector< pair_t > > topsAndFlag = graph.getWeightPairs(topName, pos);
  if (!topsAndFlag.first)
  {
    throw std::invalid_argument("unkown top");
  }
  if (topsAndFlag.second.isEmpty() || topsAndFlag.second[0].second.isEmpty())
  {
    return in;
  }
  Vector< pair_t > tops = topsAndFlag.second;
  sortPair(tops);
  if (tops[0].second.getSize() != 0)
  {
    out << tops[0].first << ' ' << tops[0].second[0];
    for (size_t j = 1; j < tops[0].second.getSize(); ++j)
    {
      out << ' ' << tops[0].second[j];
    }
  }

  for (size_t i = 1; i < tops.getSize(); ++i)
  {
    out << '\n';
    if (tops[i].second.getSize() == 0)
    {
      continue;
    }
    out << tops[i].first << ' ' << tops[i].second[0];
    for (size_t j = 1; j < tops[i].second.getSize(); ++j)
    {
      out << ' ' << tops[i].second[j];
    }
  }
  return in;
}

void malashenko::GraphsTable::inbound(std::istream& in, std::ostream& out, std::string graphName)
{
  bounds(in, out, graphName, 1);
}

void malashenko::GraphsTable::outbound(std::istream& in, std::ostream& out, std::string graphName)
{
  bounds(in, out, graphName, 0);
}

std::istream& malashenko::GraphsTable::bind(std::istream& in, std::ostream&, std::string graphName)
{
  if (!graphs_.contains(graphName))
  {
    throw std::invalid_argument("unkown graph");
  }
  Graph& graph = graphs_.at(graphName);
  std::string from;
  std::string to;
  size_t length;

  if (!(in >> from >> to >> length))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (!graph.vertexes_.contains({from, to}))
  {
    graph.vertexes_[{from, to}] = Vector< size_t >();
  }
  graph.vertexes_[{from, to}].pushBack(length);
  return in;

}


std::istream& malashenko::GraphsTable::cut(std::istream& in, std::ostream&, std::string graphName)
{
  if (!graphs_.contains(graphName))
  {
    throw std::invalid_argument("unkown graph");
  }
  Graph& graph = graphs_.at(graphName);
  std::string from;
  std::string to;
  size_t length;
  if (!(in >> from >> to >> length))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (!graph.vertexes_.contains({from, to}))
  {
    throw std::invalid_argument("unkown vertexes");
  }
  if (!graphs_.at(graphName).cutVertex(from, to, length))
  {
    throw std::invalid_argument("no vertexes to cut");
  }
  return in;

}

std::istream& malashenko::GraphsTable::create(std::istream& in, std::ostream&, std::string graphName)
{
  if (graphs_.contains(graphName))
  {
    throw std::invalid_argument("graph already exists");
  }
  size_t amount;
  if (!(in >> amount))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  Graph graph = Graph();
  for (size_t i = 0; i < amount; ++i)
  {
    std::string name;
    if (!(in >> name))
    {
      throw std::invalid_argument("input problem");
    }
    graph.addVertex(name, name, Vector< size_t >());
  }
  graphs_.insert(graphName, graph);

  return in;

}
void malashenko::GraphsTable::sortStrs(Vector< std::string >& vec)
{
  if (vec.getSize() == 0)
  {
    return;
  }
  for (size_t i = 0; i < vec.getSize() - 1; ++i)
  {
    for (size_t j = 0; j < vec.getSize() - i - 1; ++j)
    {
      if (vec[j] > vec[j + 1])
      {
        std::string temp = vec[j];
        vec[j] = vec[j + 1];
        vec[j + 1] = temp;
      }
    }
  }

}

std::istream& malashenko::GraphsTable::merge(std::istream& in, std::ostream&, std::string graphName)
{
  if (graphs_.contains(graphName))
  {
    throw std::invalid_argument("graph already exists");
  }
  std::string graph1, graph2;
  if (!(in >> graph1 >> graph2))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (!graphs_.contains(graph1) || !graphs_.contains(graph2))
  {
    throw std::invalid_argument("unkown graph");
  }
  Graph newGraph;
  copyGraph(newGraph, graphs_.at(graph1));
  copyGraph(newGraph, graphs_.at(graph2));
  graphs_.insert(graphName, newGraph);
  return in;

}

void malashenko::GraphsTable::copyGraph(Graph& newGraph, Graph& CopyGraph)
{
  for (Graph::htIter_t it = CopyGraph.vertexes_.begin(); it != CopyGraph.vertexes_.end(); ++it)
  {
    Graph::pair_t key = it->first;
    const Vector<size_t>& weights = it->second;
    if (!newGraph.vertexes_.contains(key))
    {
      newGraph.vertexes_.insert(key, Vector<size_t>());
    }
    for (size_t i = 0; i < weights.getSize(); ++i)
    {
      newGraph.vertexes_.at(key).pushBack(weights[i]);
    }
  }
}

void malashenko::GraphsTable::sortPair(Vector< pair_t >& vec)
{
  if (vec.getSize() == 0)
  {
    return;
  }
  for (size_t i = 0; i < vec.getSize() - 1; ++i)
  {
    for (size_t j = 0; j < vec.getSize() - i - 1; ++j)
    {
      if (vec[j].first > vec[j + 1].first)
      {
        pair_t temp = vec[j];
        vec[j] = vec[j + 1];
        vec[j + 1] = temp;
      }
    }
  }
}

malashenko::Vector< std::string > malashenko::GraphsTable::rmRepeatStrs(const Vector< std::string >& vec)
{
  Vector< std::string > res;
  for (size_t i = 0; i < vec.getSize(); ++i)
  {
    if (!res.contains(vec[i]))
    {
      res.pushBack(vec[i]);
    }
  }
  return res;
}

std::istream& malashenko::GraphsTable::extract(std::istream& in, std::ostream&, std::string newGraphName)
{
  if (graphs_.contains(newGraphName))
  {
    throw std::invalid_argument("graph already exists");
  }
  std::string oldGraphName;
  if (!(in >> oldGraphName))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (!graphs_.contains(oldGraphName))
  {
    throw std::invalid_argument("unknown graph");
  }
  size_t k = 0;
  if (!(in >> k))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  Vector< std::string > vertices;
  for (size_t i = 0; i < k; ++i)
  {
    std::string v;
    if (!(in >> v))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    vertices.pushBack(v);
  }

  Graph& oldGraph = graphs_.at(oldGraphName);
  for (size_t i = 0; i < vertices.getSize(); ++i)
  {
    bool found = false;
    for (Graph::htIter_t it = oldGraph.vertexes_.begin();  it != oldGraph.vertexes_.end(); ++it)
    {
      if (it->first.first == vertices[i] || it->first.second == vertices[i])
      {
        found = true;
        break;
      }
    }
    if (!found)
    {
      throw std::invalid_argument("vertex not found");
    }
  }
  Graph newGraph;
  for (Graph::htIter_t it = oldGraph.vertexes_.begin(); it != oldGraph.vertexes_.end(); ++it)
  {
    const std::string& from = it->first.first;
    const std::string& to   = it->first.second;
    bool fromFlag = false;
    bool toFlag = false;
    for (size_t i = 0; i < vertices.getSize(); ++i)
    {
      if (vertices[i] == from)
      {
        fromFlag = true;
      }
      if (vertices[i] == to)
      {
        toFlag = true;
      }
    }
    if (fromFlag && toFlag)
    {
      if (!newGraph.vertexes_.contains(it->first))
      {
        newGraph.vertexes_.insert(it->first, Vector< size_t >());
      }
      Vector< size_t >& dst = newGraph.vertexes_.at(it->first);
      const Vector< size_t >& src = it->second;
      for (size_t i = 0; i < src.getSize(); ++i)
      {
        dst.pushBack(src[i]);
      }
    }
  }
  graphs_.insert(newGraphName, newGraph);
  return in;

}
std::istream& malashenko::GraphsTable::readFile(std::istream& in)
{
  std::string graphName;

  while (in >> graphName)
  {
    size_t edgesCount;
    if (!(in >> edgesCount))
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    if (graphs_.contains(graphName))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    graphs_.insert(graphName, Graph());
    Graph &graph = graphs_.at(graphName);
    for (size_t i = 0; i < edgesCount; ++i)
    {
      std::string from;
      std::string to;
      size_t weight;
      if (!(in >> from >> to >> weight))
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      if (!graph.vertexes_.contains({from, to}))
      {
        graph.vertexes_.insert({from, to}, Vector<size_t>());
      }
      graph.vertexes_.at({from, to}).pushBack(weight);
    }
  }
  in.clear();
  return in;
}
