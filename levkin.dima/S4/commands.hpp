
#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>
#include <map>
#include "bstree.hpp"
#include <iosfwd>
#include <string>

namespace levkin {
  using DictTree = BSTree< int, std::string >;
  using DatasetStore = BSTree< std::string, DictTree >;

  using cmd_t = void (*)(std::istream&, std::ostream&, DatasetStore&);

  void
  cmdPrint(std::istream& input, std::ostream& output, DatasetStore& datasets);
  void cmdComplement(std::istream& input,
                     std::ostream& output,
                     DatasetStore& datasets);
  void cmdIntersect(std::istream& input,
                    std::ostream& output,
                    DatasetStore& datasets);
  void
  cmdUnion(std::istream& input, std::ostream& output, DatasetStore& datasets);

  bool loadDictionaries(const std::string& filename, DatasetStore& datasets);
}

#endif
