#include <utility>
#include <string>
#include <iostream>
#include <list/list.hpp>
#include "functions.hpp"
#include <limits>
namespace malashenko
{
  template< class T > class LIter;
  template< class T > class LCIter;
  template< class T > class List;

  using pair_t = std::pair< std::string, List< size_t > >;

  void getData(std::istream& in, List< pair_t >& res)
  {
    std::string name;
    while ((in >> name))
    {
      List< size_t > nums;
      size_t num;
      if (in.eof())
      {
        break;
      }
      while ((in >> num))
      {
        nums.push_back(num);
      }
      pair_t p(name, nums);
      res.push_back(p);
      in.clear();
    }
  }

  size_t getCheckedSum(size_t a, size_t b)
  {
    const size_t max_st = std::numeric_limits< size_t >::max();
    if (max_st - a > b)
    {
      return a + b;
    }
    throw std::overflow_error("overflow");
  }

  size_t getMaxSeqSize(const List< pair_t >& list)
  {
    size_t maxSize = 0;
    for (LIter< pair_t > s = list.begin(); s != list.end(); ++s)
    {
      maxSize = std::max(maxSize, (*s).second.size());
    }
    return maxSize;
  }

  void printSeqNames(std::ostream& out, const List< pair_t >& list)
  {
    LIter< pair_t > s = list.begin();
    out << (*s).first;
    ++s;
    for (; s != list.end(); ++s)
    {
      out << ' ' << (*s).first;
    }
    out << '\n';
  }

  void getTransedSeq(List< List< size_t > >& res, const List< pair_t >& list)
  {
    size_t maxSize = getMaxSeqSize(list);
    for (size_t curSize = 1; curSize < maxSize + 1; ++curSize)
    {
      List< size_t > newSeq;
      for (LIter< pair_t > s = list.begin(); s != list.end(); ++s)
      {
        if ((*s).second.size() < curSize)
        {
          continue;
        }
        LIter< size_t > valNode = ((*s).second.begin()) + (curSize - 1);
        newSeq.push_back(*valNode);
      }
      res.push_back(newSeq);
    }
  }

  void printNewSeqs(std::ostream& out, const List< List< size_t > >& list)
  {
    for (LIter< List< size_t > > outStart = list.begin(); outStart != list.end(); ++outStart)
    {
      LIter< size_t > inStart = (*outStart).begin();
      out << *inStart;
      ++inStart;
      for (; inStart != (*outStart).end(); ++inStart)
      {
        out << ' ' << *inStart;
      }
      out << '\n';
    }
  }

  void countSums(const List< List< size_t > >& list, List< size_t >& sums)
  {
    for (LIter< List< size_t > > outStart = list.begin(); outStart != list.end(); ++outStart)
    {
      size_t sum = 0;
      for (LIter< size_t > inStart = (*outStart).begin(); inStart != (*outStart).end(); ++inStart)
      {
        sum = getCheckedSum(sum, *inStart);
      }
      sums.push_back(sum);
    }
  }

  void printSums(std::ostream& out, const List< size_t >& sums)
  {
    if (sums.empty())
    {
      return;
    }

    LIter< size_t > s = sums.begin();
    out << *s;
    ++s;
    for (; s != sums.end(); ++s)
    {
      out << ' ' << *s;
    }
    out << '\n';
  }
}
