#include <fstream>
#include <iostream>
#include <limits>
#include <string>

#include "commands.hpp"
#include "graph.hpp"
#include <hashtable.hpp>
#include <vector.hpp>

int main(int args, char ** argv)
{
  if (args != 2) {
    std::cerr << "filename is required, count of args must be 1\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "could not open file\n";
    return 1;
  }

  std::istream & input = std::cin;
  std::ostream & output = std::cout;

  chernov::Graphs graphs;

  std::string graph_name;
  while (file >> graph_name) {
    size_t edges_count;
    file >> edges_count;
    for (size_t i = 0; i < edges_count; ++i) {
      std::string start, end;
      size_t weight;
      file >> start >> end >> weight;
      graphs.addEdge(graph_name, start, end, weight);
    }
  }

  chernov::HashTable< std::string, chernov::cmd_t, std::hash< std::string >, std::equal_to< std::string > > cmds(64);
  cmds.add("graphs", chernov::cmdGraphs);
  cmds.add("vertexes", chernov::cmdVertexes);
  cmds.add("outbound", chernov::cmdOutbound);
  cmds.add("inbound", chernov::cmdInbound);
  cmds.add("bind", chernov::cmdBind);
  cmds.add("cut", chernov::cmdCut);

  std::string cmd;
  while (input >> cmd) {
    try {
      cmds.at(cmd)(input, output, graphs);
    } catch (const std::out_of_range & e) {
      output << "<INVALID COMMAND>\n";
      std::streamsize max_streamsize = std::numeric_limits< std::streamsize >::max();
      input.ignore(max_streamsize, '\n');
    }
  }
  if (!input.eof()) {
    std::cerr << "bad input\n";
    return 1;
  }
}
