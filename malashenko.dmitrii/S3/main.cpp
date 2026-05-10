#include "hash_table.hpp"
#include "hash_func.hpp"
#include "hash_table_iter.hpp"
#include "graphs_table.hpp"

#include <string>
#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    std::cerr << "Invalid arguments\n";
    return 1;
  }

  std::ifstream file(argv[1]);

  if (!file)
  {
    std::cerr << "Cannot open file\n";
    return 1;
  }

  malashenko::GraphsTable table;

  try
  {
    table.readFile(file);
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }

  using cmd_t = void (malashenko::GraphsTable::*)( std::istream &, std::ostream &, std::string);

  malashenko::HashTable< std::string, cmd_t, malashenko::HmacHash< std::string >, malashenko::Equal< std::string > > commands;

  commands.add("graphs", &malashenko::GraphsTable::graphs);
  commands.add("vertexes", &malashenko::GraphsTable::vertexes);
  commands.add("outbound", &malashenko::GraphsTable::outbound);
  commands.add("inbound", &malashenko::GraphsTable::inbound);
  commands.add("bind", &malashenko::GraphsTable::bind);
  commands.add("cut", &malashenko::GraphsTable::cut);
  commands.add("create", &malashenko::GraphsTable::create);

  std::string command;

  while (std::cin >> command)
  {
    if (!commands.has(command))
    {
      std::cout << "<INVALID COMMAND>\n";
      while (std::cin.peek() != '\n' && std::cin.peek() != EOF)
      {
        std::cin.get();
      }

      continue;
    }

    std::string graphName;

    if (command != "graphs")
    {
      if (!(std::cin >> graphName))
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
    }

    try
    {
      cmd_t func = commands.get(command);

      (table.*func)(std::cin, std::cout, graphName);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";

      while (std::cin.peek() != '\n' && std::cin.peek() != EOF)
      {
        std::cin.get();
      }
    }
  }

  return 0;
}
