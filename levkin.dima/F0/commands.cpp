#include "commands.hpp"
#include "node.hpp"
#include <iostream>
namespace rl {
  void healthcheck(std::istream&, std::ostream& out, RootDB&, RLRoot*)
  {
    out << "Works fine\n";
  }

  Cmds getCmds()
  {
    Cmds cmds;
    cmds["healthcheck"] = healthcheck;
    return cmds;
  }

  void RootDB::select(RLRoot& item) { selected = &item; }
  void RootDB::select_by_index(size_t index) { selected = &data[index]; }
  RLRoot::RLRoot(float width, float height)
      : baseWidth(width), baseHeight(height)
  {
    root.id = "root";
    root.width = width;
    root.height = height;

    mapOfNodes.add("root", &root);
  }
}
