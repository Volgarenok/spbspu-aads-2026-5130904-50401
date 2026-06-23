#include <exception>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include "graphs-table.hpp"
#include "hash-func.hpp"
#include "hash-table.hpp"
#include "input-utils.hpp"

int main(int argc, char ** argv)
{
  using namespace kondrat;

  if (argc != 2)
  {
    std::cerr << "invalid arguments\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "cannot open file\n";
    return 1;
  }

  GraphsTable table;
  try
  {
    table.readFile(file);
  }
  catch (const std::exception & exception)
  {
    std::cerr << exception.what() << '\n';
    return 1;
  }

  using CommandFunction = bool (GraphsTable::*)(std::istream &, std::ostream &, const std::string &);
  HashTable< std::string, CommandFunction, blake2 > commands;
  commands.add("graphs", &GraphsTable::graphs);
  commands.add("vertexes", &GraphsTable::vertexes);
  commands.add("outbound", &GraphsTable::outbound);
  commands.add("inbound", &GraphsTable::inbound);
  commands.add("bind", &GraphsTable::bind);
  commands.add("cut", &GraphsTable::cut);
  commands.add("create", &GraphsTable::create);
  commands.add("merge", &GraphsTable::merge);
  commands.add("extract", &GraphsTable::extract);

  std::string commandName;
  while (std::cin >> commandName)
  {
    std::string graphName;
    if (commandName != "graphs" && !detail::readArgument(std::cin, graphName))
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      continue;
    }

    try
    {
      const CommandFunction command = commands.at(commandName);
      const bool hasOutput = (table.*command)(std::cin, std::cout, graphName);
      if (hasOutput)
      {
        std::cout << '\n';
      }
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
    }

    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}
