#include "commands.hpp"
#include "bstree.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
int main(int argc, char* argv[])
{
  if (argc < 2) {
    std::cerr << "Error: filename parameter is required.\n";
    return 1;
  }
  levkin::DatasetStore datasets;
  levkin::BSTree< std::string, levkin::cmd_t > cmds;
  cmds.push("print", levkin::cmdPrint);
  cmds.push("union", levkin::cmdUnion);
  std::string line;
  while (std::getline(std::cin, line)) {
    std::stringstream ss(line);
    std::string cmd_name;
    ss >> cmd_name;
    if (cmds.has(cmd_name)) {
      auto func = cmds.get(cmd_name);
      func(ss, std::cout, datasets);
      if (ss.fail()) {
        std::cout << "<error>\n";
        std::cin.clear();
      }
    } else {
      std::cout << "<error>\n";
    }
  }
  return 0;
}
