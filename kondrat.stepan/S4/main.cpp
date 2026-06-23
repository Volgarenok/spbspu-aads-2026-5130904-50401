#include <fstream>
#include <iostream>
#include <limits>
#include <string>

#include "bstree.hpp"
#include "datasets.hpp"

int main(int argc, char ** argv)
{
  if (argc != 2)
  {
    std::cerr << "bad num of args\n";
    return 1;
  }

  std::ifstream file(argv[1]);

  if (!file.is_open())
  {
    std::cerr << "Cannot open file\n";
    return 1;
  }

  kondrat::Collection datasets;

  std::string token;
  std::string currentName;

  while (file >> token)
  {
    try
    {
      int key = std::stoi(token);

      std::string value;
      if (!(file >> value))
      {
        break;
      }

      datasets.at(currentName).push(key, value);
    }
    catch (const std::exception &)
    {
      currentName = token;

      if (!datasets.contains(currentName))
      {
        datasets.push(currentName, kondrat::Dictionary{});
      }
    }
  }

  using command_t = void (*)(std::istream &, std::ostream &, kondrat::Collection &);

  kondrat::BSTree< std::string, command_t > commands;

  commands.push("print", kondrat::print);
  commands.push("complement", kondrat::complement);
  commands.push("intersect", kondrat::intersect);
  commands.push("union", kondrat::unionCollections);

  std::string command;

  while (std::cin >> command)
  {
    try
    {
      commands.at(command)(std::cin, std::cout, datasets);
    }
    catch (const std::exception &)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  return 0;
}
