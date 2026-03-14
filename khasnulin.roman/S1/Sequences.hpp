#ifndef SEQUENCES_HPP
#define SEQUENCES_HPP

#include "List.hpp"

namespace khasnulin
{
  std::pair< std::string, BiList< size_t > > readRow(std::istream &in);

  using bilist_row_pairs = khasnulin::BiList< std::pair< std::string, khasnulin::BiList< size_t > > >;
  bilist_row_pairs readAll(std::istream &in);

  void printSequenceNames(std::ostream &out, const bilist_row_pairs &sequence);

  BiList< BiList< size_t > > getTransosedNumsSequences(const bilist_row_pairs &sequence);

  void printSequencesNumsByPlace(std::ostream &out, const BiList< BiList< size_t > > &sequence);

  void printSumsOfSequences(std::ostream &out, const BiList< BiList< size_t > > &sequence);

  size_t safeSum(size_t a, size_t b);
}

#endif
