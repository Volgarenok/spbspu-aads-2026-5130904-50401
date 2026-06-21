#include "commands.hpp"
#include "dom.hpp"
#include <iostream>
#include <string>

using namespace rl;
int main()
{
  std::string current;
  Cmds cmds = getCmds();
  RootDB db;
  while (std::cin >> current) {
    try {
      if (!cmds.has(current)) {
        throw std::logic_error("Unknown command");
      }
      cmds.at(current)(std::cin, std::cout, db);
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
