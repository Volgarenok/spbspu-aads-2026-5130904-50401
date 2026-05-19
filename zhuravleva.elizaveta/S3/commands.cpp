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
