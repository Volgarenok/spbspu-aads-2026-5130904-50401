#ifndef COMMANDS
#define COMMANDS
#include "node.hpp"
#include "hashtable.hpp"
#include <functional>
#include <iostream>
#include <string>

namespace rl {
  struct RootDB;
  template < typename Key, typename Value >
  using Map = levkin::HashTable< Key, Value >;
  using cmd_t = std::function< void(std::istream&, std::ostream&, RootDB&) >;
  using Cmds = Map< std::string, cmd_t >;

  struct RLRoot {
    RLNode root;
    Map< std::string, RLNode* > mapOfNodes;
    float base_width = 1920.0f;
    float base_height = 1080.0f;
  };
  struct RootDB {
    stf::Vector< RLRoot > data;
    RLRoot* selected = nullptr;

    void select(RLRoot& item);
    void select_by_index(size_t index);
  };

  void healthcheck(std::istream& in, std::ostream& out, RootDB& db);
  void aa(std::istream& in, std::ostream& out, RootDB& db);  
  Cmds getCmds();

}

#endif
