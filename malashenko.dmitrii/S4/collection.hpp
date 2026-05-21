#ifndef DATASETS_COLLECTION
#define DATASETS_COLLECTION
#include <iostream>
#include <string>
#include "BStree.hpp"
namespace malashenko
{
  using Dataset = BSTree< int, std::string >;
  using Collection = BSTree< std::string, Dataset >;
  void print(std::istream& in, std::ostream& out, Collection& datasets);
  void complement(std::istream& in, std::ostream& out, Collection& datasets);
  void intersect(std::istream& in, std::ostream& out, Collection& datasets);
  void unionCollections(std::istream& in, std::ostream& out, Collection& datasets);
}

#endif