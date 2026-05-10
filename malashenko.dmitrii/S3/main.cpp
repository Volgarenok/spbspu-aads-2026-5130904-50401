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
  commands.add("merge", &GraphsTable::merge);
  commands.add("extract", &GraphsTable::extract);

  std::string cmd;

  while (std::cin >> cmd)
  {
    std::string graphName;
    if (cmd != "graphs")
    {
      if (!(std::cin >> graphName))
      {
        std::cout << "<INVALID COMMAND>\n";
        long toignore = std::numeric_limits<std::streamsize>::max();
        std::cin.ignore(toignore, '\n');
        continue;
      }
    }

    try
    {
      (table.*commands.get(cmd))(std::cin, std::cout, graphName);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

  }

  return 0;
}
