#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "bstree.hpp"

#include <iosfwd>
#include <string>

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

  using Dictionary = BSTree< int, std::string, IntCompare >;
  using Storage = BSTree< std::string, Dictionary, StringCompare >;

  void print(std::ostream & out, std::istream & in, Storage & storage);
  void complement(std::ostream & out, std::istream & in, Storage & storage);
  void intersect(std::ostream & out, std::istream & in, Storage & storage);
  void unionDicts(std::ostream & out, std::istream & in, Storage & storage);

  void loadStorage(Storage & storage, const std::string & filename);
}

#endif
