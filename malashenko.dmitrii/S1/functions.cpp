#include <utility>
#include <string>
#include <iostream>
#include "list.hpp"
#include "functions.hpp"
#include <limits>
namespace malashenko
{
  template< class T > class LIter;
  template< class T > class LCIter;
  template< class T > class List;

  using pair_t = std::pair< std::string, List< int > >;

  void getData(std::istream& in, List< pair_t >& res)
  {
    std::string name;
    while (!in.eof())
    {
      in >> name;
      List< int > nums;
      unsigned long long num;



      if (!(in >> num))
      {
        if (in.eof())
        {
          break;
        }
        if (num > std::numeric_limits< int >::max())
        {
          throw std::overflow_error("Too big number");
        }
        res.push_back({name, nums});
        in.clear(in.rdstate() ^ std::ios_base::failbit);
        continue;
      }

      nums.push_back(static_cast< size_t >(num));
      while ((in >> num))
      {
        if (num > std::numeric_limits< int >::max())
        {
          throw std::overflow_error("Too big number");
        }
        nums.push_back(static_cast< size_t >(num));
      }
      if (num > std::numeric_limits< int >::max())
      {
        throw std::overflow_error("Too big number");
      }
      pair_t p(name, nums);
      res.push_back(p);
      in.clear(in.rdstate() ^ std::ios_base::failbit);
    }
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

  void printSeqsAndGetSums(std::ostream& out, const List< pair_t >& list, List< int >& sums)
  {
    size_t maxSize = getMaxSeqSize(list);
    for (size_t curSize = 1; curSize < maxSize + 1; ++curSize)
    {
      size_t sum = 0;
      LIter< pair_t > s = list.begin();
      if ((*s).second.size() >= curSize)
      {
        LIter< int > valNode = ((*s).second.begin()) + (curSize - 1);

        sum += *valNode;
        out << *valNode;
        ++s;
      }

      for (; s != list.end(); ++s)
      {
        if ((*s).second.size() < curSize)
        {
          continue;
        }

        LIter< int > valNode = ((*s).second.begin()) + (curSize - 1);
        sum += *valNode;
        if (curSize != maxSize)
        {
          out << ' ';
        }
        out << *valNode;
      }
      out << '\n';
      sums.push_back(sum);
    }
  }

  void printSums(std::ostream& out, const List< int >& sums)
  {
    if (sums.empty())
    {
      return;
    }

    LIter< int > s = sums.begin();
    out << *s;
    ++s;
    for (; s != sums.end(); ++s)
    {
      out << ' ' << *s;
    }
    out << '\n';
  }
}
