#ifndef DATASETS_HPP
#define DATASETS_HPP

#include <iosfwd>
#include <string>
#include "bstree.hpp"

namespace kondrat
{
  using Dictionary = BSTree< int, std::string >;
  using Collection = BSTree< std::string, Dictionary >;

  void print(std::istream & in, std::ostream & out, Collection & datasets);
  void complement(std::istream & in, std::ostream & out, Collection & datasets);
  void intersect(std::istream & in, std::ostream & out, Collection & datasets);
  void unionCollections(std::istream & in, std::ostream & out, Collection & datasets);
}

#endif
