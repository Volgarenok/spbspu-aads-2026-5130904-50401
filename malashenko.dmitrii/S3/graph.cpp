#include "graph.hpp"

malashenko::Vector< std::string > malashenko::Graph::getTops()
{
  malashenko::Vector< std::string > tops;
  for (htIter_t start = vertexes_.begin(); start != vertexes_.end(); ++start)
  {
    tops.pushBack(start->first.first);
    tops.pushBack(start->first.second);
  }
  return tops;
}

malashenko::Vector< std::pair< std::string, malashenko::Vector< size_t > > > malashenko::Graph::getWeightPairs(const std::string& topName, size_t pos)
{
  malashenko::Vector< std::pair< std::string, malashenko::Vector< size_t > > > weightPairs;
  for (htIter_t start = vertexes_.begin(); start != vertexes_.end(); ++start)
  {
    if ((!pos ? start->first.first : start->first.second) == topName)
    {
      malashenko::Vector< size_t > weights = start->second;
      sort(weights);
      weightPairs.pushBack({!pos ? start->first.second : start->first.first, weights});
    }
  }
  return weightPairs;
}

bool malashenko::Graph::cutVertex(const std::string& from, const std::string& to, size_t size)
{
  malashenko::Vector< size_t >& vec = vertexes_.get({from, to});
  for (size_t i = 0; i < vec.getSize(); ++i)
  {
    if (vec[i] == size)
    {
      vec.erase(i);
      return true;
    }
  }
  return false;
}

void malashenko::Graph::sort(malashenko::Vector< size_t >& vec)
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
        size_t temp = vec[j];
        vec[j] = vec[j + 1];
        vec[j + 1] = temp;
      }
    }
  }
}
