#ifndef FUNCTIONS
#define FUNCTIONS
#include <utility>
#include <string>
#include <iostream>
#include "list.hpp"
#include "iterator.hpp"
namespace malashenko
{
  template< class T > class LIter;
  template< class T > class LCIter;
  template< class T > class List;

  using pair_t = std::pair< std::string, List< int > >;

  void getData(std::istream& in, List< pair_t >& res);
  size_t getMaxSeqSize(const List< pair_t >& list);
  void printSeqNames(std::ostream& out, const List< pair_t >& list);
  void printSeqsAndGetSums(std::ostream& out, const List< pair_t >& list, List< int >& sums);
  void printSums(std::ostream& out, const List< int >& sums);
}

#endif
