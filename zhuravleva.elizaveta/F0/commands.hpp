#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <string>
#include "taskManager.hpp"

namespace zhuravleva
{
  void executeCommand(const std::string& command,
      std::istream& in, std::ostream& out, TaskManager& manager);
}

#endif
