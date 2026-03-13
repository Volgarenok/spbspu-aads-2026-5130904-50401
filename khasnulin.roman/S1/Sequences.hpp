#ifndef SEQUENCES_HPP
#define SEQUENCES_HPP

#include <iosfwd>

#include "List.hpp"

namespace khasnulin
{
  std::pair< std::string, BiList< int > > readRow(std::istream &in)
  {
    std::string name;
    BiList< int > list;
    in >> name;
    return std::pair< std::string, BiList< int > >{name, list};
  }
}

#endif
