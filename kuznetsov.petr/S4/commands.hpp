#ifndef COMMANDSS4_HPP
#define COMMANDSS4_HPP
#include <BSTree.hpp>
#include <cstddef>
#include <iosfwd>

namespace kuznetsov {
  using record = BSTree< size_t, std::string, std::less< size_t > >;
  using dicts = BSTree< std::string, record, std::less< std::string > >;

  void print(std::ostream&, std::istream&, dicts& ds);
  void complement(std::ostream&, std::istream&, dicts& ds);
  void intersect(std::ostream&, std::istream&, dicts& ds);
  void unionDicts(std::ostream&, std::istream&, dicts& ds);
}
#endif
