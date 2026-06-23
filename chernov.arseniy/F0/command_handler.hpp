#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include <string>
#include <cuckooht.hpp>
#include <vector.hpp>
#include "hashers.hpp"
#include "tree_manager.hpp"

namespace chernov {
  class CommandHandler {
  public:
    using CommandFunc = void (*)(Vector< std::string > &, TreeManager &, std::ostream &);
    using CmdTable = CuckooHT< std::string, CommandFunc, Hasher1, Hasher2, std::equal_to< std::string > >;

    explicit CommandHandler(TreeManager & manager);
    void handle(const std::string & line);

  private:
    TreeManager & manager_;
    CmdTable commands_;
  };
}

#endif
