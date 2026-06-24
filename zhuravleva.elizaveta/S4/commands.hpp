#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iosfwd>
#include <string>
#include "bstree.hpp"

namespace zhuravleva
{
  using Dictionary = BSTree< int, std::string >;
  using DictionaryStorage = BSTree< std::string, Dictionary >;

  void print(
    std::ostream& out,
    const std::string& name,
    const DictionaryStorage& dictionaries
  );

  void complement(
    DictionaryStorage& dictionaries,
    const std::string& newName,
    const std::string& firstName,
    const std::string& secondName
  );

  void intersect(
    DictionaryStorage& dictionaries,
    const std::string& newName,
    const std::string& firstName,
    const std::string& secondName
  );

  void unite(
    DictionaryStorage& dictionaries,
    const std::string& newName,
    const std::string& firstName,
    const std::string& secondName
  );
}

#endif
