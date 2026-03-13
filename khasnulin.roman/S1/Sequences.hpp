#ifndef SEQUENCES_HPP
#define SEQUENCES_HPP

#include "List.hpp"

namespace khasnulin
{
  std::pair< std::string, BiList< int > > readRow(std::istream &in);

  BiList< std::pair< std::string, BiList< int > > > readAll(std::istream &in);

  void printSequenceNames(std::ostream &out, BiList< std::pair< std::string, BiList< int > > > &sequence);
}

#endif
