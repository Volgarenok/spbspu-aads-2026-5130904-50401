#ifndef FUNCTIONS
#define FUNCTIONS
#include <utility>
#include <string>
#include <iostream>
#include "list.hpp"
#include "iterator.hpp"
namespace malashenko {
  template< class T >
  class LIter;
  template< class T >
  class LCIter;
  template< class T >
  class List;

  using pair_t = std::pair<std::string, List< int > >;
  
  List< pair_t > getSequences(std::istream& in)
  {
    List< pair_t > res;
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
    return res;
  }

  size_t maxSeqSize(const List< pair_t >& list)
  {
    size_t maxSize = 0;
    for (LIter< pair_t > s = list.begin(), f = list.end(); s != f; ++s)
    {
      maxSize = std::max(maxSize, (*s).second.size());
    }
    maxSize = std::max(maxSize, list.back().second.size());

    return maxSize;
  }

  void printNames(const List< pair_t >& list)
  {
    for (LIter< pair_t > s = list.begin(), f = list.end(); s != f; ++s)
    {
      std::cout << (*s).first << ' ';
    }
    std::cout << list.back().first << '\n';
  }

  void printSeqs(const List< pair_t >& list)
  {
    size_t maxSize = maxSeqSize(list);
    // List< List< int > > res;
    for (size_t curSize = 1; curSize != maxSize + 1; ++curSize)
    {
      // List< int > seq;
      for (LIter< pair_t > s = list.begin(), f = list.end(); s != f; ++s)
      {
        if ((*s).second.size() < curSize)
        {
          continue;
        }

        LIter< int > valNode = ((*s).second.begin()) + (curSize - 1);
        std::cout << *valNode << ' ';
      }

      if (list.back().second.size() < curSize)
      {
        std::cout << '\n';
        continue;
      }
      LIter< int > valNode = list.back().second.begin() + (curSize - 1);
      std::cout << *valNode << '\n';
    }

  }
}

#endif
