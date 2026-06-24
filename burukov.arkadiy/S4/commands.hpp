#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>
#include "bstree.hpp"

namespace burukov
{
  using Dict = BSTree< int, std::string >;
  using Dicts = BSTree< std::string, Dict >;

  void cmdPrint(std::istream& in, std::ostream& out, Dicts& dicts);
  void cmdComplement(std::istream& in, std::ostream& out, Dicts& dicts);
  void cmdIntersect(std::istream& in, std::ostream& out, Dicts& dicts);
  void cmdUnion(std::istream& in, std::ostream& out, Dicts& dicts);
}

#endif
