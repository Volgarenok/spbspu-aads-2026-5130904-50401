#include "commands.hpp"
#include <iostream>
#include <unordered_map>

namespace rl {
  void healthcheck(std::istream&, std::ostream& out, Database&)
  {
    out << "Works fine\n";
  }

  Cmds getCmds()
  {
    Cmds cmds;
    cmds["healthcheck"] = healthcheck;
    return cmds;
  }
}
