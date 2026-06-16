#include <iostream>
#include <exception>
#include <fstream>
#include "commands.hpp"
#include "graph.hpp"
#include "input.hpp"

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
    lukashevich::processCommands(std::cin, std::cout, graphs, commands);
  } catch (const std::exception & err) {
    std::cerr << err.what() << '\n';
    return 1;
  }

  return 0;
}
