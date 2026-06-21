#ifndef COMMANDS
#define COMMANDS
#include "dom.hpp"
#include "hashtable.hpp"
#include <functional>
#include <iostream>
#include <string>
namespace rl {
  struct RootDB {
    stuff::Vector< RTRoot > data;
    RTRoot* selected = nullptr;

    void select(RTRoot& item);
    void select_by_index(size_t index);
  };

  template < typename Key, typename Value >
  using Map = levkin::HashTable< Key, Value >;
  using cmd_t = std::function< void(std::istream&, std::ostream&, RootDB&) >;
  using Cmds = Map< std::string, cmd_t >;
  void healthcheck(std::istream& in, std::ostream& out, RootDB& db);
  Cmds getCmds();

}

#endif
