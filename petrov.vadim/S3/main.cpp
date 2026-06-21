#include <iostream>
#include <fstream>
#include <sstream>
#include "FuncManage.hpp"

int main(int argc, char const *argv[])
{
  using namespace petrov;
  if (argc != 2)
  {
    std::cerr << "arg invalid\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "file open invalid\n";
    return 1;
  }

  FuncManage table;
  try
  {
    table.readFile(file);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }

  using cmd_t = void (FuncManage::*)(std::ostream &, std::istream &, std::string);
  HashTable< std::string, cmd_t, sha1, Equal< std::string > > commands(16);

  commands.add("graphs", &FuncManage::graphs);
  commands.add("vertexes", &FuncManage::vertexes);
  commands.add("outbound", &FuncManage::outbound);
  commands.add("inbound", &FuncManage::inbound);
  commands.add("bind", &FuncManage::bind);
  commands.add("cut", &FuncManage::cut);
  commands.add("create", &FuncManage::create);
  commands.add("merge", &FuncManage::merge);
  commands.add("extract", &FuncManage::extract);

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
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        continue;
      }
    }

    std::string args;
    std::getline(std::cin, args);
    std::istringstream input(args);

    try
    {
      cmd_t command = commands.get(cmd);
      (table.*command)(std::cout, input, graphName);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
