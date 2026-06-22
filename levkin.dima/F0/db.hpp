#ifndef DB_RL
#define DB_RL

#include "node.hpp"
#include "utils.hpp"
#include <memory>
#include <string>

namespace rl {

struct RLRootNode {
  std::string name;
  std::string filePath;
  RLNode root;
  Map<std::string, RLNode *> mapOfNodes;

  RLRootNode() {
    root.id = "root";
    mapOfNodes.add("root", &root);
  }
  RLRootNode(float w, float h) : name("unnamed"), filePath("unnamed.rl") {
    root.id = "root";
    root.width = w;
    root.height = h;
    mapOfNodes.add("root", &root);
  }
  RLRootNode(std::string layoutName, std::string path, float w, float h)
      : name(std::move(layoutName)), filePath(std::move(path)) {
    root.id = "root";
    root.width = w;
    root.height = h;
    mapOfNodes.add("root", &root);
  }
};

class RootDB {
private:
  Map<std::string, std::unique_ptr<RLRootNode>> storage;
  RLRootNode *selected = nullptr;

public:
  bool loadDatabase(const std::string &dbPath);
  bool saveDatabase(const std::string &dbPath);
  bool loadLayout(const std::string &name);
  bool saveActiveLayout();
  void createNewLayout(const std::string &name, const std::string &path,
                       float w, float h);
  void selectLayout(const std::string &name);
  RLRootNode *getActive() const;
  bool hasLayout(const std::string &name) const;
};

} // namespace rl

#endif
