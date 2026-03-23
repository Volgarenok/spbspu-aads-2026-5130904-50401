#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "list.hpp"
#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>

namespace lukashevich
{
  using pair_t = std::pair< std::string, List< size_t > >;

  void getData(std::istream& in, List< pair_t >& res);
  void printSeqName(const List< pair_t >& data);
  bool hasNum(const List< pair_t >& data);
  size_t printOneRow(List< pair_t >& data);
  void printAll(List< pair_t >& data, List< size_t >& sums);
  void printSum(const List< size_t >& sums);
}

#endif
