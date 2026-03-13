#ifndef SEQUENCES_HPP
#define SEQUENCES_HPP

#include "List.hpp"

namespace khasnulin
{
  std::pair< std::string, BiList< int > > readRow(std::istream &in);

  using bilist_row_pairs = khasnulin::BiList< std::pair< std::string, khasnulin::BiList< int > > >;
  bilist_row_pairs readAll(std::istream &in);

  void printSequenceNames(std::ostream &out, const bilist_row_pairs &sequence);

  void printSequencesNumsByPlace(std::ostream &out, const bilist_row_pairs &sequence);
}

#endif
