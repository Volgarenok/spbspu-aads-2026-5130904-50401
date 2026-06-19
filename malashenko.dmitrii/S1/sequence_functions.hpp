#ifndef SEQUENCE_FUNCTIONS_HPP
#define SEQUENCE_FUNCTIONS_HPP
#include <iostream>
#include <string>
#include <utility>
#include "list.hpp"
#include "iterator.hpp"
namespace malashenko
{
  using pair_t = std::pair< std::string, List< size_t > >;

  void getData(std::istream& in, List< pair_t >& res);
  size_t getMaxSeqSize(const List< pair_t >& list);
  void printSeqNames(std::ostream& out, const List< pair_t >& list);
  void getTransedSeq(List< List< size_t > >& res, const List< pair_t >& list);
  void printNewSeqs(std::ostream& out, const List< List< size_t > >& list);
  void countSums(const List< List< size_t > >& list, List< size_t >& sums);
  void printSums(std::ostream& out, const List< size_t >& sums);
  size_t getCheckedSum(size_t a, size_t b);
}

#endif
