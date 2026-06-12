#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>
#include "list.hpp"

namespace lukashevich
{
  using pair_t = std::pair< std::string, List< size_t > >;

  void getData(std::istream& in, List< pair_t >& res);
  void printSeqName(std::ostream& out, const List< pair_t >& data);
  bool hasNum(const List< pair_t >& data);
  size_t printOneRow(std::ostream& out, List< pair_t >& data);
  void printSum(std::ostream& out, const List< size_t >& sums);
}

#endif
