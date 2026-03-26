#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "list.hpp"

namespace donkeev
{
  void read(List< std::pair< std::string, List< size_t > > >&, std::istream&);
  void printInfo(const List< std::pair< std::string, List< size_t > > >&, std::ostream&);
}
#endif