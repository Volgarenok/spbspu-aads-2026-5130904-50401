#include <iostream>
#include <string>
#include "command_handler.hpp"
#include "tree_manager.hpp"

int main()
{
  chernov::TreeManager manager;
  chernov::CommandHandler handler(manager);
  std::string line;

  while (std::getline(std::cin, line)) {
    handler.handle(line);
  }
}
