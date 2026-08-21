#include <iostream>
#include <sstream>
#include "commands.hpp"
#include "bstree.hpp"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Error: filename parameter is required.\n";
    return 1;
  }
  petrov::DatasetStore datasets;
  if (!petrov::loadDictionaries(argv[1], datasets)) {
    std::cerr << "Error: could not open file " << argv[1] << "\n";
    return 1;
  }
  petrov::CmdStore cmds;
  cmds.push("print", petrov::cmdPrint);
  cmds.push("complement", petrov::cmdComplement);
  cmds.push("intersect", petrov::cmdIntersect);
  cmds.push("union", petrov::cmdUnion);

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
          std::cout << "<INVALID COMMAND>\n";
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
