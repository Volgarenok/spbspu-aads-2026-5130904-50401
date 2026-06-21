#include "commands.hpp"
#include "node.hpp"
#include <iostream>
#include <string>

using namespace rl;
int main()
{
  std::string cmd;
  Cmds cmds = getCmds();
  RootDB db;
  
  while (std::cin >> cmd) {
    try {
      if (!cmds.has(cmd)) {
        throw std::logic_error("Unknown command");
      }
      cmds.at(cmd)(std::cin, std::cout, db, db.selected);
    } catch (const std::exception& e) {
      std::cout << "<INVALID COMMAND>\n";
      std::string dummy;
      std::getline(std::cin, dummy);
    }
  }

  if (!std::cin.eof() && std::cin.fail()) {
    std::cerr << "Bad input\n";
    return 1;
  }
  return 0;
}
