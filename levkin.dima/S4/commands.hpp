
#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>
#include <map>
#include "bstree.hpp"

namespace levkin {

  using BST = levkin::BSTree< std::string, std::string >;
  using DB = std::map< std::string, BST >;
  void handleError(std::ostream& out, std::istream& in);
  using cmd_t = void (*)(std::istream&, std::ostream&, DB&);

  void loadDatasets(std::istream& in, DB& datasets);
  void printDataset(std::istream& in, std::ostream& out, DB& datasets);
  void complementDatasets(std::istream& in, std::ostream& out, DB& datasets);
  void intersectDatasets(std::istream& in, std::ostream& out, DB& datasets);
  void unionDatasets(std::istream& in, std::ostream& out, DB& datasets);}

#endif
