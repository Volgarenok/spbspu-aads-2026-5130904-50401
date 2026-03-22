#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "list.hpp"
#include <iostream>
#include <string>
#include <stdexcept>

namespace lukashevich
{
  using pair_t = std::pair< std::string, List< size_t > >;

  void getData(std::istream& in, List< pair_t >& res);

  void printSum(const List< size_t >& sums);
}

#endif
