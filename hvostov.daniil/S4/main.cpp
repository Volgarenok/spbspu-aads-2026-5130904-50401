#include <iostream>
#include <fstream>
#include <limits>
#include "commands.hpp"

int main(int argc, char* argv[])
{
  using namespace hvostov;
  if (argc < 2) {
    std::cerr << "No filename in arguments\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Cant open file\n";
    return 1;
  }

  DatasetMap datasets;

  using cmd_t = void (*)(std::istream&, std::ostream&, DatasetMap&);
  hvostov::BSTree< std::string, cmd_t > cmds;

  using const_cmd_t = void (*)(std::istream&, std::ostream&, const DatasetMap&);
  hvostov::BSTree< std::string, const_cmd_t > constCmds;

  constCmds.push("print", printDataset);

  cmds.push("complement", complementDatasets);
  cmds.push("intersect", intersectDatasets);
  cmds.push("union", unionDatasets);

  try {
    loadDatasets(file, datasets);
  } catch (const std::exception& e) {
    std::cerr << "Error loading datasets: " << e.what() << "\n";
    return 1;
  }

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      if (constCmds.has(cmd)) {
        constCmds.at(cmd)(std::cin, std::cout, datasets);
        std::cout << "\n";
      } else {
        cmds.at(cmd)(std::cin, std::cout, datasets);
      }
    } catch (const std::exception&) {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }
  }

  if (!std::cin.eof()) {
    std::cerr << "Bad input!\n";
    return 1;
  }
  return 0;
}
