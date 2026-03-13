#ifndef SEQUENCES_HPP
#define SEQUENCES_HPP

#include <iosfwd>

#include "List.hpp"

namespace khasnulin
{
  std::pair< std::string, BiList< int > > readRow(std::istream &in);
}

#endif
