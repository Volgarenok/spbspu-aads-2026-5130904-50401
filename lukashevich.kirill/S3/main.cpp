#include <iostream>
#include <exception>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "commands.hpp"
#include "graph.hpp"
#include "input.hpp"

namespace {
  const char * INVALID_COMMAND = "<INVALID COMMAND>";
}

int main(int argc, char ** argv)
{
  if (argc != 2) {
    std::cerr << "bad argument\n";
    return 1;
  }

  std::ifstream input(argv[1]);

  if (!input) {
    std::cerr << "cant open file\n";
    return 1;
  }

  try {
    lukashevich::GraphTable graphs(251, 8);
    lukashevich::readGraphs(input, graphs);
    const lukashevich::CommandTable commands = lukashevich::createCommandTable();

    std::string line;

    while (std::getline(std::cin, line)) {
      std::string commandName;
      std::istringstream commandInput(line);

      if (!(commandInput >> commandName)) {
        continue;
        }

      try {
        if (!commands.has(commandName)) {
          throw std::logic_error("unknown command");
        }

        const bool hasOutput = commands.at(commandName)(commandInput,
            graphs, std::cout);

        if (lukashevich::hasExtraData(commandInput)) {
          throw std::logic_error("extra command data");
        }

        if (hasOutput) {
          std::cout << '\n';
        }
      } catch (const std::exception &) {
        std::cout << INVALID_COMMAND << '\n';
      }
    }
  } catch (const std::exception & err) {
    std::cerr << err.what() << '\n';
    return 1;
  }
  return 0;
}
