#include "commands.hpp"
#include "db.hpp"
#include <boost/filesystem.hpp>
#include <iostream>
namespace rl {
  void initLayout(std::istream& in, std::ostream& out, RootDB& db, RLRootNode*)
  {
    std::string layoutName;
    if (!(in >> layoutName)) {
      out << "Error: init requires a layout name\n";
      return;
    }

    std::string filePath;
    if (in >> filePath) {
      db.createNewLayout(
          layoutName, filePath, config::DEFAULT_WIDTH, config::DEFAULT_HEIGHT);

      // db.loadFromFile(filePath);
    } else {
      std::string defaultPath = generateLayoutPath(layoutName);
      db.createNewLayout(
          layoutName, defaultPath, config::DEFAULT_WIDTH,
          config::DEFAULT_HEIGHT);
    }

    db.selectLayout(layoutName);

    out << "Layout '" << layoutName << "' initialized successfully.\n";
  }
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
  void healthcheck(std::istream&, std::ostream& out, RootDB&, RLRootNode*)
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

  void initLayoutCommand(
      std::istream& in, std::ostream& out, RootDB& db, RLRootNode*)
  {
    float w = config::DEFAULT_WIDTH;
    float h = config::DEFAULT_HEIGHT;

    if (!(in >> w >> h)) {
      out << "Error: Parent ID required.\n";
      return;
    }

    std::string name = rl::generateLayoutName(w, h);
    std::string path = rl::generateLayoutPath(name);

    db.createNewLayout(name, path, w, h);

    out << "Layout successfully initialized with size: " << w << "x" << h
        << "\n";
  }
  void
  createChild(std::istream& in, std::ostream& out, RootDB&, RLRootNode* layout)
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
  createNode(std::istream& in, std::ostream& out, RootDB&, RLRootNode* layout)
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

  void
  emptyNode(std::istream& in, std::ostream& out, RootDB&, RLRootNode* layout)
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
  void
  deleteNode(std::istream& in, std::ostream& out, RootDB&, RLRootNode* layout)
  {
    if (!layout) {
      out << "Error: No layout initialized.\n";
      return;
    }

    std::string id;
    if (!(in >> id))
      return;

    if (id == "root") {
      out << "Error: Cannot delete root node.\n";
      return;
    }
    if (!layout->mapOfNodes.has(id)) {
      out << "Error: Node '" << id << "' not found.\n";
      return;
    }

    RLNode* nodeToDelete = layout->mapOfNodes.get(id);
    RLNode* parentNode = nodeToDelete->parent;

    unregisterSubtree(nodeToDelete, layout->mapOfNodes);
    parentNode->removeChild(id);

    out << "Node '" << id << "' deleted successfully.\n";
  }

  void
  moveNode(std::istream& in, std::ostream& out, RootDB&, RLRootNode* layout)
  {
    if (!layout) {
      out << "Error: No layout initialized.\n";
      return;
    }

    std::string id, newParentId;
    if (!(in >> id >> newParentId))
      return;

    if (!layout->mapOfNodes.has(id)) {
      out << "Error: Node '" << id << "' not found.\n";
      return;
    }

    RLNode* nodeToMove = layout->mapOfNodes.get(id);
    RLNode* oldParent = nodeToMove->parent;
    RLNode* newParent = (newParentId == "null")
                            ? &layout->root
                            : layout->mapOfNodes.get(newParentId);

    if (!newParent) {
      out << "Error: New parent '" << newParentId << "' not found.\n";
      return;
    }

    RLNode* checkNode = newParent;
    while (checkNode) {
      if (checkNode == nodeToMove) {
        out << "Error: Cannot move node into its own subtree.\n";
        return;
      }
      checkNode = checkNode->parent;
    }

    std::unique_ptr< RLNode > movedPtr;
    for (size_t i = 0; i < oldParent->children.getSize(); ++i) {
      if (oldParent->children[i]->id == id) {
        movedPtr = std::move(oldParent->children[i]);
        oldParent->children.erase(i);
        break;
      }
    }

    newParent->addChild(std::move(movedPtr));
    out << "Node '" << id << "' moved to '" << newParent->id << "'.\n";
  }

  void
  setWidth(std::istream& in, std::ostream& out, RootDB&, RLRootNode* layout)
  {
    if (!layout) {
      out << "Error: No layout initialized.\n";
      return;
    }
    std::string id;
    float val;
    if (in >> id >> val) {
      if (!layout->mapOfNodes.has(id)) {
        out << "Error: Not found.\n";
        return;
      }
      layout->mapOfNodes.get(id)->width = val;
      out << "Node '" << id << "' width updated.\n";
    }
  }

  void
  setHeight(std::istream& in, std::ostream& out, RootDB&, RLRootNode* layout)
  {
    if (!layout) {
      out << "Error: No layout initialized.\n";
      return;
    }
    std::string id;
    float val;
    if (in >> id >> val) {
      if (!layout->mapOfNodes.has(id)) {
        out << "Error: Not found.\n";
        return;
      }
      layout->mapOfNodes.get(id)->height = val;
      out << "Node '" << id << "' height updated.\n";
    }
  }

  void setSize(std::istream& in, std::ostream& out, RootDB&, RLRootNode* layout)
  {
    if (!layout) {
      out << "Error: No layout initialized.\n";
      return;
    }
    std::string id;
    float w, h;
    if (in >> id >> w >> h) {
      if (!layout->mapOfNodes.has(id)) {
        out << "Error: Not found.\n";
        return;
      }
      RLNode* node = layout->mapOfNodes.get(id);
      node->width = w;
      node->height = h;
      out << "Node '" << id << "' size updated to " << w << "x" << h << ".\n";
    }
  }
  void printHelp(std::istream&, std::ostream& out, RootDB&, RLRootNode*)
  {
    out << "Available commands:\n";

    Cmds cmds = getCmds();

    for (auto it = cmds.begin(); it != cmds.end(); ++it) {
      out << "  - " << it.key() << "\n";
    }
  }
  Cmds getCmds()
  {
    Cmds cmds;
    cmds["help"] = printHelp;
    cmds["healthcheck"] = healthcheck;
    cmds["init"] = initLayout;
    cmds["create_child"] = createChild;
    cmds["create"] = createNode;
    cmds["empty"] = emptyNode;
    cmds["delete"] = deleteNode;
    cmds["move"] = moveNode;
    cmds["set_width"] = setWidth;
    cmds["set_height"] = setHeight;
    cmds["set_size"] = setSize;
    return cmds;
  }
}
