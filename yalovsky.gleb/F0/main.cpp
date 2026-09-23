#include <exception>
#include <iostream>
#include "commands.hpp"
#include "graph-storage.hpp"

int main(int argc, char**)
{
  if (argc != 1)
  {
    std::cerr << "This program does not accept command line arguments\n";
    return 1;
  }

  try
  {
    yalovsky::GraphStorage storage;
    yalovsky::runCommands(std::cin, std::cout, storage);
  }
  catch (const std::exception& error)
  {
    std::cerr << "Internal error: " << error.what() << '\n';
    return 2;
  }

  return 0;
}
