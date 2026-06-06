#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "list.hpp"

namespace donkeev
{
  void read(List< std::pair< std::string, List< size_t > > >&, std::istream&);
  bool areAnyvalues(const List< std::pair< std::string, List< size_t > > >&);
  size_t getMaxSize(const List< std::pair< std::string, List< size_t > > >&);
  bool checkOnOverflow(const List< std::pair< std::string, List< size_t > > >&);
  void printInfo(const List< std::pair< std::string, List< size_t > > >&, std::ostream&);
}
#endif
