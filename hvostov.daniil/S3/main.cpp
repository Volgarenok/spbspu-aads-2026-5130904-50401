#include <fstream>
#include <iostream>
#include <limits>
#include "commands.hpp"
#include "graph.hpp"
#include "hash_table.hpp"
#include "sip_hash.hpp"

void handleError(std::ostream& out, std::istream& in)
{
  out << "<INVALID COMMAND>\n";
  auto toignore = std::numeric_limits< std::streamsize >::max();
  in.ignore(toignore, '\n');
}

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
    try {
      graphs.add(graph_name, g);
    } catch (...) {
      graphs.rehash();
      graphs.add(graph_name, g);
    }
  }
  input.close();

  using cmd_t = void (*)(std::istream&, std::ostream&, hvostov::GraphTable&);
  hvostov::HashTable< std::string, cmd_t, hvostov::SipHash< std::string >, std::equal_to< std::string > > cmds(9);
  cmds.add("graphs", hvostov::graphs);
  cmds.add("vertexes", hvostov::vertexes);
  cmds.add("outbound", hvostov::outbound);
  cmds.add("inbound", hvostov::inbound);
  cmds.add("bind", hvostov::bind);
  cmds.add("cut", hvostov::cut);
  cmds.add("create", hvostov::create);
  cmds.add("merge", hvostov::merge);
  cmds.add("extract", hvostov::extract);

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      if (cmds.has(cmd)) {
        cmds.at(cmd)(std::cin, std::cout, graphs);
      } else {
        handleError(std::cout, std::cin);
      }
    } catch (const std::exception&) {
      handleError(std::cout, std::cin);
    }
  }

  if (!std::cin.eof()) {
    std::cerr << "Bad input!\n";
    return 1;
  }

  return 0;
}
