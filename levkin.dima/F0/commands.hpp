#ifndef COMMANDS
#define COMMANDS
#include "dom.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
namespace rl {
  // под индексом 0 - выбранный 
  using cmd_t = void (*)(std::istream&, std::ostream&, Database&);
  using Cmds = std::unordered_map< std::string, cmd_t >;

  void healthcheck(std::istream& in, std::ostream& out, Database& db);

  Cmds getCmds();
}

#endif
