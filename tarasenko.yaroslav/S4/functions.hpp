#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <iosfwd>
#include <string>
#include "binary_tree.hpp"

namespace tarasenko
{
  using Dataset = BSTree< int, std::string >;
  using Datasets = BSTree< std::string, Dataset >;
  using Command = void (*)(std::istream&, std::ostream&, Datasets&);

  Datasets getFromFile(std::istream& input);
  void print(std::istream& in, std::ostream& out, Datasets& datasets);
  void complement(std::istream& in, std::ostream&, Datasets& datasets);
  void intersect(std::istream& in, std::ostream&, Datasets& datasets);
  void unionFunc(std::istream& in, std::ostream&, Datasets& datasets);
}

#endif
