#include "commands.hpp"
#include "node.hpp"
#include <iostream>
#include <sstream>

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
  void unregisterSubtree(RLNode* node, Map< std::string, RLNode* >& mapOfNodes)
  {
    for (size_t i = 0; i < node->children.getSize(); ++i) {
      unregisterSubtree(node->children[i].get(), mapOfNodes);
    }
    mapOfNodes.drop(node->id);
  }

  void initLayout(std::istream& in, std::ostream& out, RootDB& db, RLRoot*)
  {
    float w = 1920.0f;
    float h = 1080.0f;

    std::string line;
    if (std::getline(in, line) && !line.empty()) {
      std::stringstream ss(line);
      ss >> w >> h;
    }

    RLRoot newRoot(w, h);
    db.data.pushBack(std::move(newRoot));
    db.selectByIndex(db.data.getSize() - 1);

    out << "Layout initialized: " << w << "x" << h << ". Root node created.\n";
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
