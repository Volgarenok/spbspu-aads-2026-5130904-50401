#include <iostream>
#include <string>
#include "mybst.hpp"

namespace goltsov
{
  using bst_s_bst_is = BSTree< std::string,
    BSTree< long long, std::string, std::less< long long > >, std::less< std::string > >;
  using bst_is_iterator = BSTIterator< long long, std::string >;
  using bst_func = BSTree< std::string,
    void (*)(std::ostream&, std::istream&, bst_s_bst_is&), std::less< std::string > >;

  void readSets(std::istream&, bst_s_bst_is&);

  void printParsing(std::ostream&, std::istream&, bst_s_bst_is&);
  void complementParsing(std::ostream&, std::istream&, bst_s_bst_is&);
  void intersectParsing(std::ostream&, std::istream&, bst_s_bst_is&);
  void unionParsing(std::ostream&, std::istream&, bst_s_bst_is&);

  void print_ds(std::ostream&, bst_s_bst_is&, std::string);
  void complement_ds(bst_s_bst_is&, std::string, std::string, std::string);
  void intersect_ds(bst_s_bst_is&, std::string, std::string, std::string);
  void union_ds(bst_s_bst_is&, std::string, std::string, std::string);
}
