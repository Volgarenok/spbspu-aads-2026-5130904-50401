#include "graphs_table.hpp"

void malashenko::GraphsTable::graphs(std::istream& in, std::ostream& out, std::string graphName)
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

void malashenko::GraphsTable::vertexes(std::istream& in, std::ostream& out, std::string graphName)
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
  if (!graphs_.has(graphName));
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
  if (!graphs_.has(graphName));
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
    length = std::stoi(length_str);
  }
  catch(const std::exception& e)
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
  if (!graphs_.has(graphName));
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
  if (length_str[0] == '-')
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  size_t length;
  try
  {
    length = std::stoi(length_str);
  }
  catch(const std::exception& e)
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


void malashenko::GraphsTable::create(std::istream& in, std::ostream& out, std::string graphName)
{
  if (graphs_.has(graphName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  graphs_.add(graphName, Graph());
}









