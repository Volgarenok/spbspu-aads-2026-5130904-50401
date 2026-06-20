#include <string>
#include <iostream>
#include <fstream>
#include "hash_table.hpp"
#include "hash_func.hpp"
#include "hash_table_iter.hpp"
#include "graphs_table.hpp"


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

  if (!table.readFile(file))
  {
      std::cerr << "input error\n";
      return 1;
  }

  using cmd_t = void (GraphsTable::*)( std::istream &, std::ostream &, std::string);
  using cmdWithInput_t = std::istream& (GraphsTable::*)( std::istream &, std::ostream &, std::string);

  HashTable< std::string, cmd_t, HmacHash< std::string >, Equal< std::string > > commands;
  HashTable< std::string, cmdWithInput_t, HmacHash< std::string >, Equal< std::string > > commandsWithInput;

  commands.insert("graphs", &GraphsTable::graphs);
  commands.insert("vertexes", &GraphsTable::vertexes);
  commands.insert("outbound", &GraphsTable::outbound);
  commands.insert("inbound", &GraphsTable::inbound);
  commandsWithInput.insert("bind", &GraphsTable::bind);
  commandsWithInput.insert("cut", &GraphsTable::cut);
  commandsWithInput.insert("create", &GraphsTable::create);
  commandsWithInput.insert("merge", &GraphsTable::merge);
  commandsWithInput.insert("extract", &GraphsTable::extract);

  std::string cmd;

while (std::cin >> cmd)
{
  std::string graphName;

  if (cmd != "graphs")
  {
    if (!(std::cin >> graphName))
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      continue;
    }
  }

  try
  {
    if (commands.contains(cmd))
    {
      (table.*commands.at(cmd))(std::cin, std::cout, graphName);
      std::cout << '\n';
    }
    else if (commandsWithInput.contains(cmd))
    {
      if (!(table.*commandsWithInput.at(cmd))(std::cin, std::cout, graphName))
      {
        throw std::invalid_argument("input error");
      }
    }
    else
    {
      throw std::invalid_argument("unknown command");
    }
  }
  catch (...)
  {
    std::cout << "<INVALID COMMAND>\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}

  return 0;
}
