#ifndef COMMANDS
#define COMMANDS

#include "hashtable.hpp"
#include "node.hpp"
#include "vector.hpp"
#include <iostream>
#include <string>

namespace rl {

  struct RootDB;
  struct RLRoot;

  template < typename Key, typename Value >
  using Map = levkin::HashTable< Key, Value >;

  using cmd_t = void (*)(std::istream&, std::ostream&, RootDB&, RLRoot*);
  using Cmds = Map< std::string, cmd_t >;

  struct RLRoot {
    RLRoot() = default;
    RLRoot(float width, float height);

    RLNode root;
    Map< std::string, RLNode* > mapOfNodes;
    float baseWidth = 1920.0f;
    float baseHeight = 1080.0f;
  };

  struct RootDB {
    stf::Vector< RLRoot > data;
    RLRoot* selected = nullptr;

    void select(RLRoot& item);
    void select_by_index(size_t index);
  };

  void healthcheck(std::istream&, std::ostream&, RootDB&, RLRoot*);
  void initLayout(std::istream&, std::ostream&, RootDB&, RLRoot*);
  void createChild(std::istream&, std::ostream&, RootDB&, RLRoot*);
  void createNode(std::istream&, std::ostream&, RootDB&, RLRoot*);
  void emptyNode(std::istream&, std::ostream&, RootDB&, RLRoot*);
  void deleteNode(std::istream&, std::ostream&, RootDB&, RLRoot*);
  void moveNode(std::istream&, std::ostream&, RootDB&, RLRoot*);
  void setWidth(std::istream&, std::ostream&, RootDB&, RLRoot*);
  void setHeight(std::istream&, std::ostream&, RootDB&, RLRoot*);
  void setSize(std::istream&, std::ostream&, RootDB&, RLRoot*);
  Cmds getCmds();

}

#endif
