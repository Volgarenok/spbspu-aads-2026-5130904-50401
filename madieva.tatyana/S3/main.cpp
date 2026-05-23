#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include "hash_table.hpp"
#include "graph_storage.hpp"
#include "commands.hpp"

namespace madieva {
  bool readGraphs(std::istream & file, GraphStorage& storage)
  {
    std::string graph_name;
    while (file >> graph_name) {
      size_t edges_count;
      if (!(file >> edges_count)) {
        return false;
      }
      Graph graph(graph_name);
      for (size_t i = 0; i < edges_count; ++i) {
        std::string a, b;
        size_t weight;
        if (!(file >> a >> b >> weight)) {
          return false;
        }
        graph.addEdge(a, b, weight);
      }
      try {
        storage.addGraph(graph_name, graph);
      } catch (...) {
        return false;
      }
    }
    return true;
  }
}

int main(int argc, char * argv[])
{
  if (argc != 2) {
    return 1;
  }
  madieva::GraphStorage storage;

  std::ifstream file(argv[1]);
  if (!(file.is_open())) {
    return 1;
  }
  if (!madieva::readGraphs(file, storage)) return 1;

  using Commands = void(*)(std::istream &,
    std::ostream &, madieva::GraphStorage &);
  madieva::HashTable< std::string, Commands,
    madieva::XXHash64, madieva::StringEqual> commands;
  commands.add("graphs", madieva::cmd_graphs);
  commands.add("vertexes", madieva::cmd_vertexes);
  commands.add("outbound", madieva::cmd_outbound);
  commands.add("inbound", madieva::cmd_inbound);
  commands.add("bind", madieva::cmd_bind);
  commands.add("cut", madieva::cmd_cut);
  commands.add("create", madieva::cmd_create);
  commands.add("merge", madieva::cmd_merge);
  commands.add("extract", madieva::cmd_extract);

  std::string line;
  while (std::getline(std::cin, line)) {
    std::istringstream iss(line);
    std::string cmd;
    iss >> cmd;
    if (commands.has(cmd)) {
      commands.get(cmd)(iss, std::cout, storage);
    } else {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
  return 0;
}
