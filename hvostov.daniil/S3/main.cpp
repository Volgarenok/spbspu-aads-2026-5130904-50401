#include <fstream>
#include <iostream>
#include <limits>
#include "commands.hpp"
#include "graph.hpp"
#include "hash_table.hpp"
#include "sip_hash.hpp"

int main(int argc, char* argv[])
{
  if (argc < 2) {
    std::cerr << "Too few arguments\n";
    return 1;
  }

  std::ifstream input(argv[1]);
  if (!input) {
    std::cerr << "Cannot open input: " << argv[1] << "\n";
    return 1;
  }

  hvostov::GraphTable graphs;

  std::string graph_name;
  size_t edges_count = 0;
  while (input >> graph_name >> edges_count) {
    hvostov::Graph g;
    for (size_t i = 0; i < edges_count; ++i) {
      std::string from, to;
      size_t weight;
      input >> from >> to >> weight;
      g.addEdge(from, to, weight);
    }
    graphs.add(graph_name, g);
  }
  input.close();

  hvostov::HashTable< std::string, hvostov::cmd_t, hvostov::SipHash< std::string >, std::equal_to< std::string > > cmds(
    9);
  cmds.add("bind", hvostov::bind);
  cmds.add("cut", hvostov::cut);
  cmds.add("create", hvostov::create);
  cmds.add("merge", hvostov::merge);
  cmds.add("extract", hvostov::extract);

  hvostov::HashTable< std::string, hvostov::const_cmd_t, hvostov::SipHash< std::string >, std::equal_to< std::string > >
    constCmds(9);
  constCmds.add("graphs", hvostov::graphs);
  constCmds.add("vertexes", hvostov::vertexes);
  constCmds.add("outbound", hvostov::outbound);
  constCmds.add("inbound", hvostov::inbound);

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      if (cmds.contains(cmd)) {
        cmds.at(cmd)(std::cin, std::cout, graphs);
      } else {
        constCmds.at(cmd)(std::cin, std::cout, graphs);
        std::cout << "\n";
      }
    } catch (const std::exception&) {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }
  }

  if (!std::cin.eof()) {
    std::cerr << "Bad input!\n";
    return 1;
  }

  return 0;
}
