#include "commands.hpp"
#include "dom.hpp"
#include <iostream>
#include <unordered_map>
namespace rl {
  void healthcheck(std::istream&, std::ostream& out, RootDB&)
  {
    out << "Works fine\n";
  }

  Cmds getCmds()
  {
    Cmds cmds;
    cmds["healthcheck"] = healthcheck;
    return cmds;
  }

  void RootDB::select(RTRoot& item) { selected = &item; }
  void RootDB::select_by_index(size_t index) { selected = &data[index]; }

}
