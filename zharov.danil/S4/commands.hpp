#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <fstream>
#include <bstree.hpp>

namespace zharov
{
  using dictionary_t = zharov::BSTree< int, std::string, std::less< int > >;
  using dictionaries_t = zharov::BSTree< std::string, dictionary_t, std::less< std::string > >;
  using cmd = void (*)(std::ostream&, std::istream&, dictionaries_t&);

  void print(std::ostream&, std::istream&, dictionaries_t&);
  void complement(std::ostream&, std::istream&, dictionaries_t&);
  void intersect(std::ostream&, std::istream&, dictionaries_t&);
  void makeUnion(std::ostream&, std::istream&, dictionaries_t&);

}

#endif
