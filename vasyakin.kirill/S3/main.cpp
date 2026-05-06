#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include "commands.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "Error: cannot open file '" << argv[1] << "'\n";
    return 1;
  }

  return 0;
}
