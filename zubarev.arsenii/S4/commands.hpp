#ifndef DATASET_COMMANDS_HPP
#define DATASET_COMMANDS_HPP

#include "dataset.hpp"
#include <iomanip>
#include <iosfwd>
namespace zubarev
{
  void cmd_print(std::istream& in, std::ostream& out, DatasetTable& dataset);
  void cmd_complement(std::istream& in, std::ostream& out, DatasetTable& dataset);
  void cmd_intersect(std::istream& in, std::ostream& out, DatasetTable& dataset);
  void cmd_union(std::istream& in, std::ostream& out, DatasetTable& dataset);

}

#endif
