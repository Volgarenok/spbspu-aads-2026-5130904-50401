#ifndef COMMANDS_RL
#define COMMANDS_RL

#include "db.hpp"
#include "utils.hpp"
#include <iostream>
#include <string>

namespace rl {
  using cmd_t = void (*)(std::istream&, std::ostream&, RootDB&, RLRootNode*);
  using Cmds = Map< std::string, cmd_t >;
  void bootstrapSession(RootDB&, std::ostream&);
  void healthcheck(std::istream&, std::ostream&, RootDB&, RLRootNode*);
  void initLayout(std::istream&, std::ostream&, RootDB&, RLRootNode*);
  void createChild(std::istream&, std::ostream&, RootDB&, RLRootNode*);
  void createNode(std::istream&, std::ostream&, RootDB&, RLRootNode*);
  void emptyNode(std::istream&, std::ostream&, RootDB&, RLRootNode*);
  void deleteNode(std::istream&, std::ostream&, RootDB&, RLRootNode*);
  void moveNode(std::istream&, std::ostream&, RootDB&, RLRootNode*);
  void setWidth(std::istream&, std::ostream&, RootDB&, RLRootNode*);
  void setHeight(std::istream&, std::ostream&, RootDB&, RLRootNode*);
  void setSize(std::istream&, std::ostream&, RootDB&, RLRootNode*);
  void printHelp(std::istream&, std::ostream&, RootDB&, RLRootNode*);
  Cmds getCmds();
}

#endif
