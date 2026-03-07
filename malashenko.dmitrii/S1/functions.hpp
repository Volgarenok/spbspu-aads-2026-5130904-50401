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
      std::cout << "created seq " << name << '\n';
      List< int > nums;
      size_t num;
      if (!(in >> num))
      {
        res.push_back({name, nums});
        in.clear(in.rdstate() ^ std::ios_base::failbit);
        continue;
      }
      nums.push_back(num);
      std::cout << "Pushed value " << num << '\n';

      while ((in >> num))
      {
        nums.push_back(num);
        std::cout << "Pushed value " << num << '\n';
      }
      pair_t p(name, nums);
      res.push_back(p);
      in.clear(in.rdstate() ^ std::ios_base::failbit);
    }
    return res;
  }
}


#endif