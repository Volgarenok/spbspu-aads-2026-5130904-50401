#include "graphs.hpp"
void malashenko::Graph::graphs(std::ostream& out)
{
  topit::Vector< std::string > names;
  for (htIter_t start = graphs_.begin(); start != graphs_.end(); ++start)
  {
    names.pushBack(start->first);
  }
  sortStr(names);
  for (size_t i = 0; i < names.getSize(); ++i)
  {
    out << names[i] << '\n';
  }
}

void malashenko::Graph::vertexes(std::istream& in, std::ostream& out, std::string graphName)
{
  topit::Vector< std::string > names;
  vertexes_t vert = graphs_[graphName];
  for (VertIter_t start = vert.begin(); start != vert.end(); ++start)
  {
    names.pushBack((start->first).first);
  }
  topit::Vector< std::string > uniqueNames = rmRepeatSortedStr(names);
  sortStr(uniqueNames);
  for (size_t i = 0; i < uniqueNames.getSize(); ++i)
  {
    out << uniqueNames[i] << '\n';
  }
}

void malashenko::Graph::outbound(std::istream& in, std::ostream& out, std::string graphName)
{
  std::string top = std::string();
  in >> top;
  topit::Vector< topPair_t > tops;
  vertexes_t vert = graphs_[graphName];
  for (VertIter_t start = vert.begin(); start != vert.end(); ++start)
  {
    if ((start->first).first == top)
    {
      tops.pushBack({(start->first).second, start->second});
    }
  }
  sortPair(tops);
  topit::Vector< topPair_t > uniqueTops = rmRepeatSortedPair(tops);
  for (size_t i = 0; i < uniqueTops.getSize(); ++i)
  {
    out << uniqueTops[i].first << ' ' << uniqueTops[i].second << '\n';
  }
}


void malashenko::Graph::sortStr(topit::Vector< std::string >& vec)
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
        std::string tmp = vec[j];
        vec[j] = vec[j + 1];
        vec[j + 1] = tmp;
      }
    }
  }
}

void malashenko::Graph::sortPair(topit::Vector< topPair_t >& vec)
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
        topPair_t tmp = vec[j];
        vec[j] = vec[j + 1];
        vec[j + 1] = tmp;
      }
    }
  }
}



