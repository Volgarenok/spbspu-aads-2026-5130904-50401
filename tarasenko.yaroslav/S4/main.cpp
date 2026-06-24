#include <iostream>
#include <exception>
#include <fstream>
#include <limits>
#include <string>
#include "functions.hpp"

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "1 argument expected\n";
    return 1;
  }

  std::ifstream input(argv[1]);
  if (!input)
  {
    std::cerr << "Failed to open file\n";
    return 1;
  }
  tarasenko::Datasets datasets = tarasenko::getFromFile(input);
  tarasenko::BSTree< std::string, tarasenko::Command > cmds;
  cmds.add("print", tarasenko::print);
  cmds.add("complement", tarasenko::complement);
  cmds.add("intersect", tarasenko::intersect);
  cmds.add("union", tarasenko::unionFunc);

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout, datasets);
      if (cmd == "print")
      {
        std::cout << "\n";
      }
    }
    catch (const std::exception&)
    {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.clear();
      std::cin.ignore(toignore, '\n');
    }
  }
}
