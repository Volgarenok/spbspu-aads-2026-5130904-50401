#include <fstream>
#include <iostream>
#include "commands.hpp"
#include "graph_storage.hpp"

int main(int argc, char* [])
{
  if (argc != 2)
  {
    std::cerr << "filename is required\n";
    return 1;
  }

  try
  {
    ulanova::GraphStorage storage;
    std::ifstream file("graphs.txt");
    if (!file)
    {
      std::cerr << "cannot open file\n";
      return 1;
    }
    ulanova::loadGraphs(file, storage);

    ulanova::CommandProcessor processor(storage);
    processor.run(std::cin, std::cout);
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }

  return 0;
}
