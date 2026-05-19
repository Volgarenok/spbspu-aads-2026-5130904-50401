#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "bstree.hpp"

#include <iostream>
#include <string>

namespace studilova
{
  using Dataset = BSTree< int, std::string >;
  using Datasets = BSTree< std::string, Dataset >;

  void Print(std::istream& in, std::ostream& out, Datasets& datasets);
  void Complement(std::istream& in, std::ostream& out, Datasets& datasets);
  void Intersect(std::istream& in, std::ostream& out, Datasets& datasets);
  void cmdUnion(std::istream& in, std::ostream& out, Datasets& datasets);
}

#endif
