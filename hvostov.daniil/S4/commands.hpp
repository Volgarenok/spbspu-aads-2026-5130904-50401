#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>
#include "bstree.hpp"

namespace hvostov {
  using Dataset = hvostov::BSTree< std::string, std::string >;
  using DatasetMap = hvostov::BSTree< std::string, Dataset >;

  void loadDatasets(std::istream& in, DatasetMap& datasets);
  void printDataset(std::istream& in, std::ostream& out, const DatasetMap& datasets);
  void complementDatasets(std::istream& in, std::ostream& out, DatasetMap& datasets);
  void intersectDatasets(std::istream& in, std::ostream& out, DatasetMap& datasets);
  void unionDatasets(std::istream& in, std::ostream& out, DatasetMap& datasets);
}

#endif
