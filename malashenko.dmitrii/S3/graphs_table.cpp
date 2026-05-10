#include "graphs_table.hpp"


namespace malashenko
{
  void GraphsTable::graphs(std::istream&, std::ostream& out, std::string)
  {
    Vector< std::string > tmpTops;
    for (htIter_t start = graphs_.begin(); start != graphs_.end(); ++start)
    {
      tmpTops.pushBack(start->first);
    }

    if (tmpTops.isEmpty())
    {
      out << '\n';
      return;
    }

    Vector< std::string > tops = rmRepeatStrs(tmpTops);
    sortStrs(tops);
    for (size_t i = 0; i < tops.getSize(); ++i)
    {
      out << tops[i] << '\n';
    }
  }

  void GraphsTable::vertexes(std::istream&, std::ostream& out, std::string graphName)
  {
    if (!graphs_.has(graphName))
    {
      throw std::invalid_argument("unkown graph");
    }
    Graph& graph = graphs_.get(graphName);
    Vector< std::string > tmpTops = graph.getTops();
    if (tmpTops.isEmpty())
    {
      out << '\n';
      return;
    }
    Vector< std::string > tops = rmRepeatStrs(tmpTops);
    sortStrs(tops);
    for (size_t i = 0; i < tops.getSize(); ++i)
    {
      out << tops[i] << '\n';
    }
  }

  void GraphsTable::bounds(std::istream& in, std::ostream& out, std::string graphName, size_t pos)
  {
    if (!graphs_.has(graphName))
    {
      throw std::invalid_argument("unkown graph");
    }
    Graph& graph = graphs_.get(graphName);
    std::string topName;
    if (!(in >> topName))
    {
      throw std::invalid_argument("input problem");
    }

    Vector< std::pair< std::string, Vector< size_t > > > tops = graph.getWeightPairs(topName, pos);
    if (tops.isEmpty())
    {
      throw std::invalid_argument("unknown vertex");
    }
    sortPair(tops);
    for (size_t i = 0; i < tops.getSize(); ++i)
    {
      out << tops[i].first << ' ';
      out << tops[i].second[0];
      for (size_t j = 1; j < tops[i].second.getSize(); ++j)
      {
        out << ' ' << tops[i].second[j];
      }
      out << '\n';
    }
  }

  void GraphsTable::inbound(std::istream& in, std::ostream& out, std::string graphName)
  {
    bounds(in, out, graphName, 1);
  }

  void GraphsTable::outbound(std::istream& in, std::ostream& out, std::string graphName)
  {
    bounds(in, out, graphName, 0);
  }

  void GraphsTable::bind(std::istream& in, std::ostream&, std::string graphName)
  {
    if (!graphs_.has(graphName))
    {
      throw std::invalid_argument("unkown graph");
    }
    Graph& graph = graphs_.get(graphName);

    std::string from;
    std::string to;
    std::string length_str;
    if (!(in >> from >> to >> length_str))
    {
      throw std::invalid_argument("input problem");
    }

    size_t length;
    try
    {
      length = std::stoull(length_str);
    }
    catch(...)
    {
      throw std::invalid_argument("parameter must be a number");
    }


    if (!graph.vertexes_.has({from, to}))
    {
      graph.vertexes_[{from, to}] = Vector< size_t >();
    }
    graph.vertexes_[{from, to}].pushBack(length);
  }

  void GraphsTable::cut(std::istream& in, std::ostream&, std::string graphName)
  {
    if (!graphs_.has(graphName))
    {
      throw std::invalid_argument("unkown graph");
    }
    Graph& graph = graphs_.get(graphName);

    std::string from;
    std::string to;
    std::string length_str;
    if (!(in >> from >> to >> length_str))
    {
      throw std::invalid_argument("input problem");
    }
    size_t length;
    try
    {
      length = std::stoull(length_str);
    }
    catch(...)
    {
      throw std::invalid_argument("parameter must be a number");
    }

    if (!graph.vertexes_.has({from, to}))
    {
      throw std::invalid_argument("unkown vertexes");
    }

    if (!graph.cutVertex(from, to, length))
    {
      throw std::invalid_argument("no vertexes to cut");
    }
  }


  void GraphsTable::create(std::istream& in, std::ostream&, std::string graphName)
  {
    if (graphs_.has(graphName))
    {
      throw std::invalid_argument("graph already exists");
    }

    std::string amount;
    if (!(in >> amount))
    {
      throw std::invalid_argument("input problem");
    }

    Graph graph = Graph();

    size_t amount_ull = std::stoull(amount);

    for (size_t i = 0; i < amount_ull; ++i)
    {
      std::string name;
      if (!(in >> name))
      {
        throw std::invalid_argument("input problem");
      }
      graph.addVertex(name, name, Vector< size_t >());
    }

    graphs_.add(graphName, graph);
  }

  void GraphsTable::sortStrs(Vector< std::string >& vec)
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

  void GraphsTable::merge(std::istream& in, std::ostream&, std::string graphName)
  {
    if (graphs_.has(graphName))
    {
      throw std::invalid_argument("graph already exists");
    }

    std::string graph1, graph2;
    if (!(in >> graph1 >> graph2))
    {
      throw std::invalid_argument("input problem");
    }
    if (!graphs_.has(graph1) || !graphs_.has(graph2))
    {
      throw std::invalid_argument("unkown graph");
    }

    Graph newGraph;

    copyGraph(newGraph, graphs_.get(graph1));
    copyGraph(newGraph, graphs_.get(graph2));
    graphs_.add(graphName, newGraph);
  }


  void GraphsTable::copyGraph(Graph& newGraph, Graph& CopyGraph)
  {
    for (Graph::htIter_t it = CopyGraph.vertexes_.begin(); it != CopyGraph.vertexes_.end(); ++it)
    {
      Graph::pair_t key = it->first;
      const Vector<size_t>& weights = it->second;

      if (!newGraph.vertexes_.has(key))
      {
        newGraph.vertexes_.add(key, Vector<size_t>());
      }

      for (size_t i = 0; i < weights.getSize(); ++i)
      {
        newGraph.vertexes_.get(key).pushBack(weights[i]);
      }
    }
  }
  void GraphsTable::sortPair(Vector< pair_t >& vec)
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

  Vector< std::string > GraphsTable::rmRepeatStrs(const Vector< std::string >& vec)
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

  void GraphsTable::extract(std::istream& in, std::ostream&, std::string newGraphName)
  {
    if (graphs_.has(newGraphName))
    {
      throw std::invalid_argument("graph already exists");
    }

    std::string oldGraphName;

    if (!(in >> oldGraphName))
    {
      throw std::invalid_argument("input problem");
    }

    if (!graphs_.has(oldGraphName))
    {
      throw std::invalid_argument("unknown graph");
    }

    size_t k = 0;

    if (!(in >> k))
    {
      throw std::invalid_argument("input problem");
    }

    Vector<std::string> vertices;

    for (size_t i = 0; i < k; ++i)
    {
      std::string v;
      if (!(in >> v))
      {
        throw std::invalid_argument("input problem");
      }
      vertices.pushBack(v);
    }

    Graph& oldGraph = graphs_.get(oldGraphName);

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
        if (!newGraph.vertexes_.has(it->first))
        {
          newGraph.vertexes_.add(it->first, Vector<size_t>());
        }

        Vector<size_t>& dst = newGraph.vertexes_.get(it->first);
        const Vector<size_t>& src = it->second;

        for (size_t i = 0; i < src.getSize(); ++i)
        {
          dst.pushBack(src[i]);
        }
      }
    }

    graphs_.add(newGraphName, newGraph);
  }

  void GraphsTable::readFile(std::istream& in)
  {
    std::string graphName;

    while (in >> graphName)
    {
      std::string edgesCountStr;
      if (!(in >> edgesCountStr))
      {
        throw std::invalid_argument("input problem");
      }

      size_t edgesCount = 0;

      try
      {
        edgesCount = std::stoull(edgesCountStr);
      }
      catch (...)
      {
        throw std::invalid_argument("parameter must be a number");
      }

      if (graphs_.has(graphName))
      {
        throw std::invalid_argument("graph already exists");
      }

      graphs_.add(graphName, Graph());

      Graph &graph = graphs_.get(graphName);

      for (size_t i = 0; i < edgesCount; ++i)
      {
        std::string from;
        std::string to;
        std::string weightStr;

        if (!(in >> from >> to >> weightStr))
        {
          throw std::invalid_argument("input problem");
        }

        size_t weight = 0;

        try
        {
          weight = std::stoull(weightStr);
        }
        catch (...)
        {
          throw std::invalid_argument("parameter must be a number");
        }

        if (!graph.vertexes_.has({from, to}))
        {
          graph.vertexes_.add({from, to}, Vector<size_t>());
        }

        graph.vertexes_.get({from, to}).pushBack(weight);
      }
    }
  }
}

