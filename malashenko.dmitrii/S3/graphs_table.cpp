#include "graphs_table.hpp"

void malashenko::GraphsTable::graphs(std::istream&, std::ostream& out, std::string)
{
  malashenko::Vector< std::string > tmpTops;
  for (htIter_t start = graphs_.begin(); start != graphs_.end(); ++start)
  {
    tmpTops.pushBack(start->first);
  }
  Vector< std::string > tops = rmRepeatStrs(tmpTops);
  sortStrs(tops);
  for (size_t i = 0; i < tops.getSize(); ++i)
  {
    out << tops[i] << '\n';
  }
}

void malashenko::GraphsTable::vertexes(std::istream&, std::ostream& out, std::string graphName)
{
  if (!graphs_.has(graphName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  Graph& graph = graphs_.get(graphName);
  malashenko::Vector< std::string > tmpTops = graph.getTops();
  Vector< std::string > tops = rmRepeatStrs(tmpTops);
  sortStrs(tops);
  for (size_t i = 0; i < tops.getSize(); ++i)
  {
    out << tops[i] << '\n';
  }
}

void malashenko::GraphsTable::bounds(std::istream& in, std::ostream& out, std::string graphName, size_t pos)
{
  if (!graphs_.has(graphName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  Graph& graph = graphs_.get(graphName);
  std::string topName;
  if (!(in >> topName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  malashenko::Vector< std::pair< std::string, malashenko::Vector< size_t > > > tops = graph.getWeightPairs(topName, pos);
  if (tops.isEmpty())
  {
    out << "<INVALID COMMAND>\n";
    return;
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

void malashenko::GraphsTable::inbound(std::istream& in, std::ostream& out, std::string graphName)
{
  bounds(in, out, graphName, 1);
}

void malashenko::GraphsTable::outbound(std::istream& in, std::ostream& out, std::string graphName)
{
  bounds(in, out, graphName, 0);
}

void malashenko::GraphsTable::bind(std::istream& in, std::ostream& out, std::string graphName)
{
  if (!graphs_.has(graphName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  Graph& graph = graphs_.get(graphName);

  std::string from;
  std::string to;
  std::string length_str;
  in >> from >> to >> length_str;

  size_t length;
  try
  {
    length = std::stoull(length_str);
  }
  catch(...)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }


  if (!graph.vertexes_.has({from, to}))
  {
    graph.vertexes_[{from, to}] = malashenko::Vector< size_t >();
  }
  graph.vertexes_[{from, to}].pushBack(length);
}

void malashenko::GraphsTable::cut(std::istream& in, std::ostream& out, std::string graphName)
{
  if (!graphs_.has(graphName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  Graph& graph = graphs_.get(graphName);

  std::string from;
  std::string to;
  std::string length_str;
  if (!(in >> from >> to >> length_str))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  size_t length;
  try
  {
    length = std::stoull(length_str);
  }
  catch(...)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  if (!graph.vertexes_.has({from, to}))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  if (!graph.cutVertex(from, to, length))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
}


void malashenko::GraphsTable::create(std::istream&, std::ostream& out, std::string graphName)
{
  if (graphs_.has(graphName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  graphs_.add(graphName, Graph());
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

malashenko::Vector< std::string > malashenko::GraphsTable::rmRepeatStrs(const malashenko::Vector< std::string >& vec)
{
  malashenko::Vector< std::string > res;
  for (size_t i = 0; i < vec.getSize(); ++i)
  {
    if (!res.contains(vec[i]))
    {
      res.pushBack(vec[i]);
    }
  }
  return res;
}


void malashenko::GraphsTable::readFile(std::istream& in)
{
  std::string graphName;

  while (in >> graphName)
  {
    std::string edgesCountStr;
    if (!(in >> edgesCountStr))
    {
      throw std::runtime_error("Invalid graph description");
    }

    size_t edgesCount = 0;

    try
    {
      edgesCount = std::stoull(edgesCountStr);
    }
    catch (...)
    {
      throw std::runtime_error("Invalid edges count");
    }

    if (graphs_.has(graphName))
    {
      throw std::runtime_error("Duplicate graph");
    }

    graphs_.add(graphName, Graph());

    Graph& graph = graphs_.get(graphName);

    for (size_t i = 0; i < edgesCount; ++i)
    {
      std::string from;
      std::string to;
      std::string weightStr;

      if (!(in >> from >> to >> weightStr))
      {
        throw std::runtime_error("Invalid edge");
      }

      size_t weight = 0;

      try
      {
        weight = std::stoull(weightStr);
      }
      catch (...)
      {
        throw std::runtime_error("Invalid weight");
      }

      Graph::pair_t key = { from, to };

      if (!graph.vertexes_.has(key))
      {
        graph.vertexes_.add(key, Vector<size_t>());
      }

      graph.vertexes_.get(key).pushBack(weight);
    }
  }
}
