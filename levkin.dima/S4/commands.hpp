
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

  void loadDatasets(std::istream& in, DB& datasets);
  void printDataset(std::istream& in, std::ostream& out, DB& datasets);
  void complementDatasets(std::istream& in, std::ostream& out, DB& datasets);
  void intersectDatasets(std::istream& in, std::ostream& out, DB& datasets);
  void unionDatasets(std::istream& in, std::ostream& out, DB& datasets);}

#endif
