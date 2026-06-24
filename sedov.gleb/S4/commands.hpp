#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <string>
#include "bstree.hpp"

namespace sedov
{
  using pairs = BSTree< int, std::string >;
  using trees = BSTree< std::string, pairs >;

  void print(std::istream & in, std::ostream & out, trees & trees);
  void complement(std::istream & in, std::ostream & out, trees & trees);
  void intersect(std::istream & in, std::ostream & out, trees & trees);
  void cUnion(std::istream & in, std::ostream & out, trees & trees);
}

#endif
