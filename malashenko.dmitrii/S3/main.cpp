#include "hash_table.hpp"
#include "hash_func.hpp"
#include "hash_table_iter.hpp"
#include "graphs_table.hpp"

#include <string>
#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
  using namespace malashenko;
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

  GraphsTable table;

  try
  {
    table.readFile(file);
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }

  using cmd_t = void (GraphsTable::*)( std::istream &, std::ostream &, std::string);

  HashTable< std::string, cmd_t, HmacHash< std::string >, Equal< std::string > > commands;

  commands.add("graphs", &GraphsTable::graphs);
  commands.add("vertexes", &GraphsTable::vertexes);
  commands.add("outbound", &GraphsTable::outbound);
  commands.add("inbound", &GraphsTable::inbound);
  commands.add("bind", &GraphsTable::bind);
  commands.add("cut", &GraphsTable::cut);
  commands.add("create", &GraphsTable::create);

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
