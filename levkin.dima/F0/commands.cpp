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
  void createChild(std::istream& in, std::ostream& out, RootDB&, RLRoot* layout)
  {
    if (!layout) {
      out << "Error: No layout initialized.\n";
      return;
    }
    std::string parentId, childId;
    if (!(in >> parentId)) {
      out << "Error: Parent ID required.\n";
      return;
    }
    if (!(in >> childId)) {
      childId = generateUniqueId(layout->mapOfNodes);
    }
    if (!layout->mapOfNodes.has(parentId)) {
      out << "Error: Parent node '" << parentId << "' not found.\n";
      return;
    }
    if (layout->mapOfNodes.has(childId)) {
      out << "Error: Node '" << childId << "' already exists.\n";
      return;
    }

    auto newNode = std::make_unique< RLNode >();
    newNode->id = childId;

    layout->mapOfNodes.add(childId, newNode.get());
    layout->mapOfNodes.get(parentId)->addChild(std::move(newNode));

    out << "Node '" << childId << "' added to '" << parentId << "'.\n";
  }

  void
  createNode(std::istream& in, std::ostream& out, RootDB& db, RLRoot* layout)
  {
    if (!layout) {
      out << "Error: no layout initialized\n";
      return;
    }
    std::string id;
    if (!(in >> id)) {
      id = generateUniqueId(layout->mapOfNodes);
    }

    if (layout->mapOfNodes.has(id)) {
      out << "Error: Node '" << id << "' already exists\n";
      return;
    }

    auto newNode = std::make_unique< RLNode >();
    newNode->id = id;

    layout->mapOfNodes.add(id, newNode.get());
    layout->root.addChild(std::move(newNode));

    out << "Node '" << id << "' created as child of 'root'\n";
  }
  
  void emptyNode(std::istream& in, std::ostream& out, RootDB&, RLRoot* layout)
  {
    if (!layout) {
      out << "Error: No layout initialized.\n";
      return;
    }
    std::string id;
    if (!(in >> id))
      return;

    if (!layout->mapOfNodes.has(id)) {
      out << "Error: Node '" << id << "' not found.\n";
      return;
    }
    RLNode* node = layout->mapOfNodes.get(id);
    for (size_t i = 0; i < node->children.getSize(); ++i) {
      unregisterSubtree(node->children[i].get(), layout->mapOfNodes);
    }
    node->clearChildren();
    out << "Children of '" << id << "' removed.\n";
  }
  
  Cmds getCmds()
  {
    Cmds cmds;
    cmds["healthcheck"] = healthcheck;
    return cmds;
  }

  void RootDB::select(RLRoot& item) { selected = &item; }
  void RootDB::selectByIndex(size_t index) { selected = &data[index]; }
  RLRoot::RLRoot(float width, float height)
      : baseWidth(width), baseHeight(height)
  {
    root.id = "root";
    root.width = width;
    root.height = height;

    mapOfNodes.add("root", &root);
  }
}
