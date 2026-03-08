
#include <utility>
#include <string>
#include <iostream>
#include "list.hpp"
#include "functions.hpp"
#include "iterator.hpp"
namespace malashenko {
  template< class T >
  class LIter;
  template< class T >
  class LCIter;
  template< class T >
  class List;

  using pair_t = std::pair<std::string, List< int > >;
  
  void getData(std::istream& in, List< pair_t >& res)
  {
    std::string name;
    while (!in.eof())
    {
      in >> name;
      List< int > nums;
      size_t num;
      if (!(in >> num))
      {
        res.push_back({name, nums});
        in.clear(in.rdstate() ^ std::ios_base::failbit);
        continue;
      }
      nums.push_back(num);
      while ((in >> num))
      {
        nums.push_back(num);
      }
      pair_t p(name, nums);
      res.push_back(p);
      in.clear(in.rdstate() ^ std::ios_base::failbit);
    }
  }

  size_t getMaxSeqSize(const List< pair_t >& list)
  {
    size_t maxSize = 0;
    for (LIter< pair_t > s = list.begin(), f = list.end(); s != f; ++s)
    {
      maxSize = std::max(maxSize, (*s).second.size());
    }
    maxSize = std::max(maxSize, list.back().second.size());

    return maxSize;
  }

  void printSeqNames(std::ostream& out, const List< pair_t >& list)
  {
    for (LIter< pair_t > s = list.begin(), f = list.end(); s != f; ++s)
    {
      out << (*s).first << ' ';
    }
    out << list.back().first << '\n';
  }

  void printSeqsAndGetSums(std::ostream& out, const List< pair_t >& list, List< int >& sums)
  {
    size_t maxSize = getMaxSeqSize(list);
    for (size_t curSize = 1; curSize != maxSize + 1; ++curSize)
    {
      size_t sum = 0;
      for (LIter< pair_t > s = list.begin(), f = list.end(); s != f; ++s)
      {
        if ((*s).second.size() < curSize)
        {
          continue;
        }

        LIter< int > valNode = ((*s).second.begin()) + (curSize - 1);
        sum += *valNode;
        out << *valNode << ' ';
      }

      if (list.back().second.size() < curSize)
      {
        out << '\n';
        sums.push_back(sum);
        continue;
      }

      LIter< int > valNode = list.back().second.begin() + (curSize - 1);
      out << *valNode << '\n';
      sum += *valNode;
      sums.push_back(sum);
    }
  }

  void printSums(std::ostream& out, const List< int >& sums)
  {
    if (!sums.size())
    {
      return;
    }

    for (LIter< int > s = sums.begin(), f = sums.end(); s != f; ++s)
    {
      out << *s << ' ';
    }
    out << sums.back() << '\n';
  }
}
