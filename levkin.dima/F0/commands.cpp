#include "commands.hpp"
#include "node.hpp"
#include <iostream>
namespace rl {
  std::string generateUniqueId(const Map< std::string, RLNode* >& mapOfNodes)
  {
    static size_t counter = 1;
    while (true) {
      std::string candidate = "node" + std::to_string(counter++);
      if (!mapOfNodes.has(candidate)) {
        return candidate;
      }
    }
  }
  void healthcheck(std::istream&, std::ostream& out, RootDB&, RLRoot*)
  {
    out << "Works fine\n";
  }
  void unregisterSubtree(RLNode* node, Map< std::string, RLNode* >& mapOfNodes) {
      for (size_t i = 0; i < node->children.getSize(); ++i) {
        unregisterSubtree(node->children[i].get(), mapOfNodes);
      }
      mapOfNodes.drop(node->id);
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
