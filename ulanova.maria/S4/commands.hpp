#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <string>

#include "bstree.hpp"

namespace ulanova
{
  struct IntCompare
  {
    bool operator()(int lhs, int rhs) const;
  };

  struct StringCompare
  {
    bool operator()(const std::string & lhs, const std::string & rhs) const;
  };

  using Dictionary = BSTree< int, std::string, std::less< int > >;
  using Storage = BSTree< std::string, Dictionary, std::less< std::string > >;

  void print(std::ostream & out, std::istream & in, Storage & storage);
  void complement(std::ostream & out, std::istream & in, Storage & storage);
  void intersect(std::ostream & out, std::istream & in, Storage & storage);
  void unionDicts(std::ostream & out, std::istream & in, Storage & storage);

  void loadStorage(Storage & storage, std::istream & input);
}

#endif
