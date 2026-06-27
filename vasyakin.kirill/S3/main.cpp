#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include "commands.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "Error: cannot open file '" << argv[1] << "'\n";
    return 1;
  }

  vasyakin::GraphsMap graphs(16);
  std::string graph_name;
  size_t edge_count = 0;

  while (file >> graph_name >> edge_count)
  {
    graphs.insert(std::make_pair(graph_name, vasyakin::Graph{}));

    for (size_t i = 0; i < edge_count; ++i)
    {
      std::string u, v;
      size_t w = 0;
      file >> u >> v >> w;

      auto& g = graphs.at(graph_name);
      g.addEdge(u, v, w);
    }
  }

  using cmd_t = void(*)(std::istream&, std::ostream&, vasyakin::GraphsMap&);
  vasyakin::HashTable< std::string, cmd_t, vasyakin::HMACHash, std::equal_to< std::string > > cmds(9);

  cmds.insert(std::make_pair("graphs", vasyakin::graphs));
  cmds.insert(std::make_pair("vertexes", vasyakin::vertexes));
  cmds.insert(std::make_pair("outbound", vasyakin::outbound));
  cmds.insert(std::make_pair("inbound", vasyakin::inbound));
  cmds.insert(std::make_pair("bind", vasyakin::bind));
  cmds.insert(std::make_pair("cut", vasyakin::cut));
  cmds.insert(std::make_pair("create", vasyakin::create));
  cmds.insert(std::make_pair("merge", vasyakin::merge));
  cmds.insert(std::make_pair("extract", vasyakin::extract));

  std::string cmd;
  while (std::cin >> cmd)
  {
    if (cmds.contains(cmd))
    {
      try
      {
        cmds.at(cmd)(std::cin, std::cout, graphs);
      }
      catch (const std::exception&)
      {
        std::cout << "<INVALID COMMAND>" << '\n';
      }
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
    }

    if (std::cin.fail())
    {
      std::cin.clear();
    }
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }

  return 0;
}
