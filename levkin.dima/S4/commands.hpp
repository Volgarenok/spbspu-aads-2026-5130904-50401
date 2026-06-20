
#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>
#include <map>
#include "bstree.hpp"

namespace levkin {
  using cmd_t = void (*)(std::istream&, std::ostream&, DB&);

  using Dataset = hvostov::BSTree< std::string, std::string >;
  using DatasetMap = std::map< std::string, Dataset >;
  void handleError(std::ostream& out, std::istream& in);

  void loadDatasets(std::istream& in, DatasetMap& datasets);
  void printDataset(std::istream& in, std::ostream& out, DatasetMap& datasets);
  void complementDatasets(std::istream& in, std::ostream& out, DatasetMap& datasets);
  void intersectDatasets(std::istream& in, std::ostream& out, DatasetMap& datasets);
  void unionDatasets(std::istream& in, std::ostream& out, DatasetMap& datasets);}

#endif

cmds.push("print", printDataset);
cmds.push("complement", complementDatasets);
cmds.push("intersect", intersectDatasets);
cmds.push("union", unionDatasets);
