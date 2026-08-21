#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <string>
#include "bstree.hpp"

namespace petrov {
  using SubTree = BSTree< int, std::string >;
  using DatasetStore = BSTree< std::string, SubTree >;
  using cmd_t = void (*)(std::istream&, std::ostream&, DatasetStore&);
  using CmdStore = BSTree< std::string, cmd_t >;

  bool loadDictionaries(const std::string& filename, DatasetStore& datasets);
  void cmdPrint(std::istream& in, std::ostream& out, DatasetStore& datasets);
  void cmdComplement(std::istream& in, std::ostream& out, DatasetStore& datasets);
  void cmdIntersect(std::istream& in, std::ostream& out, DatasetStore& datasets);
  void cmdUnion(std::istream& in, std::ostream& out, DatasetStore& datasets);
}

#endif
