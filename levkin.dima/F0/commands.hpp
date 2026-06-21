#ifndef COMMANDS
#define COMMANDS
#include "dom.hpp"
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

namespace rl {
  struct RootDB {
    stuff::Vector< RTRoot > data;
    RTRoot* selected = nullptr;

    void select(RTRoot& item);
    void select_by_index(size_t index);
  };

  using cmd_t = std::function< void(std::istream&, std::ostream&, RootDB&) >;
  using Cmds = std::unordered_map< std::string, cmd_t >;
  void healthcheck(std::istream& in, std::ostream& out, RootDB& db);
  Cmds getCmds();

}

#endif
