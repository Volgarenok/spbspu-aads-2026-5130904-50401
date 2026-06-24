#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include "commands.hpp"
#include "graph.hpp"
#include "hash_table.hpp"
#include "siphash.hpp"

int main(int argc, char ** argv)
{
  if (argc != 2) {
    std::cerr << "bad num of args" << '\n';
    return 1;
  }

  sedov::graphSet graphs(16);
  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "Cannot open file" << '\n';
    return 1;
  }

  std::string graph_name;
  size_t edge_count = 0;
  while (file >> graph_name >> edge_count)
  {
    sedov::Graph g;
    for (size_t i = 0; i < edge_count; ++i)
    {
      std::string v1, v2;
      size_t w = 0;
      file >> v1 >> v2 >> w;
      g.addEdge(v1, v2, w);
    }
    graphs.add(graph_name, std::move(g));
  }
  file.close();

  using cmd_t = void (*)(std::istream &, std::ostream &, sedov::graphSet &);
  sedov::HashTable< std::string, cmd_t, sedov::Hasher< std::string >, std::equal_to< std::string > > commands(16);

  commands.add("graphs", sedov::graphs);
  commands.add("vertexes", sedov::vertexes);
  commands.add("outbound", sedov::outbound);
  commands.add("inbound", sedov::inbound);
  commands.add("bind", sedov::bind);
  commands.add("cut", sedov::cut);
  commands.add("create", sedov::create);
  commands.add("merge", sedov::merge);
  commands.add("extract", sedov::extract);

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      commands.at(cmd)(std::cin, std::cout, graphs);
    }
    catch (const std::exception &)
    {
      std::cout << "<INVALID COMMAND>" << '\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  return 0;
}
