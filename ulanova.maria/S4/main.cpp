#include "commands.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace
{
  void processLine(ulanova::Storage & storage, const std::string & line)
  {
    std::istringstream input(line);

    std::string command;
    if (!(input >> command))
    {
      return;
    }

    if (command == "print")
    {
      ulanova::print(std::cout, input, storage);
    }
    else if (command == "complement")
    {
      ulanova::complement(std::cout, input, storage);
    }
    else if (command == "intersect")
    {
      ulanova::intersect(std::cout, input, storage);
    }
    else if (command == "union")
    {
      ulanova::unionDicts(std::cout, input, storage);
    }
    else
    {
      throw std::runtime_error("invalid command");
    }

    std::string extra;
    if (input >> extra)
    {
      throw std::runtime_error("invalid command");
    }
  }
}

int main(int argc, char ** argv)
{
  if (argc < 2)
  {
    std::cerr << "filename is required\n";
    return 1;
  }

  ulanova::Storage storage;

  try
  {
    ulanova::loadStorage(storage, argv[1]);

    std::string line;
    while (std::getline(std::cin, line))
    {
      try
      {
        processLine(storage, line);
      }
      catch (...)
      {
        std::cout << "<INVALID COMMAND>\n";
      }
    }
  }
  catch (const std::exception & error)
  {
    std::cerr << error.what() << '\n';
    return 1;
  }

  return 0;
}
