#include <iostream>
#include <fstream>
#include "commands.hpp"

int main(int argc, char* argv[])
{
  using namespace hvostov;
  if (argc < 2) {
    std::cerr << "No filename in arguments\n";
    return 1;
  }
  DatasetMap datasets;
  using cmd_t = void (*)(std::istream&, std::ostream&, DatasetMap&);
  hvostov::BSTree< std::string, cmd_t > cmds;
  cmds.push("print", printDataset);
  cmds.push("complement", complementDatasets);
  cmds.push("intersect", intersectDatasets);
  cmds.push("union", unionDatasets);
  try {
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
      std::cerr << "Cant open file\n";
      return 1;
    }
    loadDatasets(file, datasets);
  } catch (const std::exception& e) {
    std::cerr << "Error loading datasets: " << e.what() << "\n";
    return 1;
  }
  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmd_t func = cmds.get(cmd);
      func(std::cin, std::cout, datasets);
    } catch (const std::runtime_error&) {
      handleError(std::cout, std::cin);
    } catch (const std::exception& e) {
      handleError(std::cout, std::cin);
    }
  }
  if (!std::cin.eof()) {
    std::cerr << "Bad input!\n";
    return 1;
  }
  return 0;
}
