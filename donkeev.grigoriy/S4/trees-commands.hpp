#ifndef TREES_COMMANDS_HPP
#define TREES_COMMANDS_HPP

#include <string>
#include "BSTree.hpp"

namespace donkeev
{
  using Dataset = BSTree< int, std::string, Comp< int > >;
  using Datasets = BSTree< std::string, Dataset, Comp< std::string > >;

  void printDicts(std::istream&, std::ostream&, donkeev::Datasets&);
  void complementDicts(std::istream&, std::ostream&, donkeev::Datasets&);
  /*void intersectDicts(std::istream&, std::ostream&, donkeev::Datasets&);
  void uniteDicts(std::istream&, std::ostream&, donkeev::Datasets&);*/
}
#endif
