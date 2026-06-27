#include "commands.hpp"
#include "bstree.hpp"
#include <iostream>
#include <sstream>
int main(int argc, char* argv[])
{
  if (argc < 2) {
    std::cerr << "Error: filename parameter is required.\n";
    return 1;
  }
  levkin::DatasetStore datasets;
  if (!levkin::loadDictionaries(argv[1], datasets)) {
    std::cerr << "Error: could not open file " << argv[1] << "\n";
    return 1;
  }
  levkin::CmdStore cmds;
  cmds.push("print", levkin::cmdPrint);
  cmds.push("complement", levkin::cmdComplement);
  cmds.push("intersect", levkin::cmdIntersect);
  cmds.push("union", levkin::cmdUnion);
  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    std::stringstream ss(line);
    std::string cmd_name;
    ss >> cmd_name;
    if (cmds.has(cmd_name)) {
      try {
        auto func = cmds.get(cmd_name);
        func(ss, std::cout, datasets);
        if (ss.fail()) {
          std::cout << "<INVALID COMAND>\n";
          std::cin.clear();
        }
      } catch (...) {
        std::cout << "<INVALID COMMAND>\n";
      }
    } else {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
  return 0;
}
