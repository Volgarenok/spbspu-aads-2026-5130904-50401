#include <fstream>
#include <iostream>
#include "commands.hpp"
#include "graph.hpp"

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "Not enough args\n";
    return 1;
  }

  std::ifstream input(argv[1]);
  if (!input.is_open())
  {
    std::cerr << "Can not open file\n";
    return 1;
  }

  using cmd = void (*)(std::ostream&, std::istream&, zharov::graphs_table&);
  using constCmd = void (*)(std::ostream&, std::istream&, const zharov::graphs_table&);
  zharov::HashTable< std::string, cmd, zharov::Blake2Hasher< std::string >,
    std::equal_to< std::string > >
    cmds;
  zharov::HashTable< std::string, constCmd, zharov::Blake2Hasher< std::string >,
    std::equal_to< std::string > >
    constCmds;

  constCmds.add("graphs", zharov::graphs);
  constCmds.add("vertexes", zharov::vertexes);
  constCmds.add("outbound", zharov::outbound);
  constCmds.add("inbound", zharov::inbound);
  cmds.add("bind", zharov::bind);
  cmds.add("cut", zharov::cut);
  cmds.add("create", zharov::create);
  cmds.add("merge", zharov::merge);
  cmds.add("extract", zharov::extract);

  zharov::graphs_table graphs;
  std::string graph_name;
  size_t edges_count;
  while (input >> graph_name >> edges_count)
  {
    zharov::Graph g(edges_count);
    for (size_t i = 0; i < edges_count; ++i)
    {
      zharov::key_t k;
      size_t weight;
      input >> k.first >> k.second >> weight;
      g.addEdge(k.first, k.second, weight);
    }
    graphs.add(graph_name, g);
  }

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      if (cmds.contains(command))
      {
        cmds.at(command)(std::cout, std::cin, graphs);
      }
      else
      {
        constCmds.at(command)(std::cout, std::cin, graphs);
        std::cout << '\n';
      }
    }
    catch (...)
    {
      std::cin.clear();
      std::cout << "<INVALID COMMAND>\n";
      std::streamsize skip = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(skip, '\n');
    }
  }
}
