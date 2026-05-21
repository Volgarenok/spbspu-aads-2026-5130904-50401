#ifndef FUNCTIONS
#define FUNCTIONS
#include <utility>
#include <string>
#include <iostream>
#include <list/list.hpp>
#include <list/iterator.hpp>
namespace malashenko
{
  template< class T > class LIter;
  template< class T > class LCIter;
  template< class T > class List;

  using pair_t = std::pair< std::string, List< size_t > >;

  void getData(std::istream& in, List< pair_t >& res);
  size_t getMaxSeqSize(const List< pair_t >& list);
  void printSeqNames(std::ostream& out, const List< pair_t >& list);
  void getTransedSeq(List< List< size_t > >& res, const List< pair_t >& list);
  void printNewSeqs(std::ostream& out, const List< List< size_t > >& list);
  void countSums(const List< List< size_t > >& list, List< size_t >& sums);
  void printSums(std::ostream& out, const List< size_t >& sums);
}

#endif
