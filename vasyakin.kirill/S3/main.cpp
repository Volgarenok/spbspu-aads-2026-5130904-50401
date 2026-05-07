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
  if (!file.is_open())
  {
    std::cerr << "Error: cannot open file '" << argv[1] << "'\n";
    return 1;
  }

  vasyakin::GraphsMap graphs(16);
  std::string graph_name;
  size_t edge_count = 0;

  while (file >> graph_name >> edge_count)
  {
    graphs.add(graph_name, vasyakin::Graph{});

    for (size_t i = 0; i < edge_count; ++i)
    {
      std::string u, v;
      size_t w = 0;
      file >> u >> v >> w;

      auto& g = graphs.get(graph_name);
      if (g.adj.has(u))
      {
        g.adj.get(u).push_back({v, w});
      }
      else
      {
        vasyakin::List< vasyakin::Edge > edges;
        edges.push_back({v, w});
        g.adj.add(u, std::move(edges));
      }
    }
  }
  file.close();

  using cmd_t = void(*)(std::istream&, std::ostream&, vasyakin::GraphsMap&);
  vasyakin::HashTable< std::string, cmd_t, vasyakin::HMACHash, std::equal_to< std::string > > cmds(9);

  cmds.add("graphs", vasyakin::graphs);
  cmds.add("vertexes", vasyakin::vertexes);
  cmds.add("outbound", vasyakin::outbound);
  cmds.add("inbound", vasyakin::inbound);
  cmds.add("bind", vasyakin::bind);
  cmds.add("cut", vasyakin::cut);
  cmds.add("create", vasyakin::create);
  cmds.add("merge", vasyakin::merge);
  cmds.add("extract", vasyakin::extract);

  std::string cmd;
  while (std::cin >> cmd)
  {
    if (cmds.has(cmd))
    {
      try
      {
        cmds.get(cmd)(std::cin, std::cout, graphs);
      }
      catch (...)
      {
        std::cout << "<INVALID COMMAND>" << '\n';
      }
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
    }

    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    else
    {
      if (std::cin.peek() != '\n' && std::cin.peek() != EOF)
      {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }
  }

  return 0;
}
