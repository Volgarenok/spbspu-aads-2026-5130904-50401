#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <bstree.hpp>

namespace chernov {
  using dict_t = chernov::BSTree< int, std::string, std::less< int > >;
  using dicts_t = chernov::BSTree< std::string, dict_t, std::less< std::string > >;
  using cmd_t = void(*)(std::istream & input, std::ostream & output, dicts_t & dicts);

  void cmdPrint(std::istream & input, std::ostream & output, dicts_t & dicts);
}

#endif
