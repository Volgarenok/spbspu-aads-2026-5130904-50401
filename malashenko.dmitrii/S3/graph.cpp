#include "graph.hpp"

namespace malashenko
{
  Vector< std::string > Graph::getTops()
  {
    Vector< std::string > tops;
    for (htIter_t start = vertexes_.begin(); start != vertexes_.end(); ++start)
    {
      tops.pushBack(start->first.first);
      tops.pushBack(start->first.second);
    }
    return tops;
  }

  std::pair< bool, Vector< Graph::pairWeight_t > > Graph::getWeightPairs(const std::string& topName, size_t pos)
  {
    Vector< std::pair< std::string, Vector< size_t > > > weightPairs;
    bool isFound = false;
    for (htIter_t start = vertexes_.begin(); start != vertexes_.end(); ++start)
    {
      if ((!pos ? start->first.first : start->first.second) == topName)
      {
        Vector< size_t > weights = start->second;
        if (!weights.isEmpty())
        {
          sort(weights);

        }
        weightPairs.pushBack({!pos ? start->first.second : start->first.first, weights});
        isFound = true;
      }
    }
    return {isFound, weightPairs};
  }

  bool Graph::cutVertex(const std::string& from, const std::string& to, size_t size)
  {
    if (!vertexes_.has({from, to}))
    {
      return false;
    }
    Vector< size_t >& vec = vertexes_.get({from, to});
    for (size_t i = 0; i < vec.getSize(); ++i)
    {
      if (vec[i] == size)
      {
        vec.erase(i);
        if (vec.isEmpty())
        {
          vertexes_.drop({from, to});
          vertexes_.add({from, from}, Vector< size_t >());
        }
        return true;
      }
    }
    return false;
  }

  void Graph::addVertex(const std::string& from, const std::string& to, Vector< size_t > vec)
  {
    vertexes_.add({from, to}, vec);
  }

  void Graph::sort(Vector< size_t >& vec)
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

}
