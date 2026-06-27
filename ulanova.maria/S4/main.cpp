#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

#include "commands.hpp"

int main(int argc, char ** argv)
{
  if (argc < 2)
  {
    std::cerr << "filename is required\n";
    return 1;
  }

  std::ifstream input(argv[1]);
  if (!input)
  {
    std::cerr << "file open error\n";
    return 1;
  }

  ulanova::Storage storage;
  ulanova::loadStorage(storage, input);

  using CommandFunc = void(*)(std::ostream &, std::istream &, ulanova::Storage &);
  ulanova::BSTree< std::string, CommandFunc, std::less< std::string > > commands;

  commands.push("print", ulanova::print);
  commands.push("complement", ulanova::complement);
  commands.push("intersect", ulanova::intersect);
  commands.push("union", ulanova::unionDicts);

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      auto it = commands.find(command);
      if (it == commands.cend())
      {
        throw std::runtime_error("invalid command");
      }
      it->second(std::cout, std::cin, storage);
      std::cout << '\n';
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  return 0;
}
