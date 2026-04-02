#ifndef YALOVSKY_INPUT_OUTPUT_HPP
#define YALOVSKY_INPUT_OUTPUT_HPP

#include <cstddef>
#include <iosfwd>
#include <string>
#include <utility>

#include "list.hpp"

namespace yalovsky
{
  using NumberList = List< std::size_t >;
  using NamedList = std::pair< std::string, NumberList >;
  using SequenceList = List< NamedList >;
  using Matrix = List< NumberList >;

  void readSequences(std::istream& in, SequenceList& sequences);
  void printNames(std::ostream& out, const SequenceList& sequences);
  void transposeSequences(const SequenceList& sequences, Matrix& result);
  void printMatrix(std::ostream& out, const Matrix& matrix);
  void calculateSums(const Matrix& matrix, NumberList& sums);
  void printNumberList(std::ostream& out, const NumberList& numbers);
}

#endif
