#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include "commands.hpp"
#include "graph.hpp"
int main(int args, char** argv)
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
  std::istream& in = std::cin;
  std::ostream& out = std::cout;
  levkin::DB graphs;
  std::string graph_name;
  while (file >> graph_name) {
    graphs.createGraph(graph_name);
    size_t edges_count = 0;
    if (!(file >> edges_count)) {
      break;
    }
    for (size_t i = 0; i < edges_count; ++i) {
      std::string start, end;
      size_t weight = 0;
      if (file >> start >> end >> weight) {
        graphs.addEdge(graph_name, start, end, weight);
      }
    }
  }
  levkin::HashTable< std::string,
                     levkin::cmd_t,
                     levkin::Sha1Hasher< std::string >,
                     levkin::KeyComp >
      cmds(64, 4);
  cmds.add("graphs", levkin::cmdGraphs);
  cmds.add("vertexes", levkin::cmdVertexes);
  cmds.add("outbound", levkin::cmdOutbound);
  cmds.add("inbound", levkin::cmdInbound);
  cmds.add("bind", levkin::cmdBind);
  cmds.add("cut", levkin::cmdCut);
  cmds.add("create", levkin::cmdCreate);
  cmds.add("merge", levkin::cmdMerge);
  cmds.add("extract", levkin::cmdExtract);
  std::streamsize max_streamsize
      = std::numeric_limits< std::streamsize >::max();
  std::string cmd;
  while (in >> cmd) {
    try {
      cmds.at(cmd)(in, out, graphs);
      if (in.fail()) {
        out << "<INVALID COMMAND>\n";
        in.clear();
        in.ignore(max_streamsize, '\n');
      }
    } catch (const std::out_of_range& e) {
      out << "<INVALID COMMAND>\n";
      in.clear();
      in.ignore(max_streamsize, '\n');
    }
  }
  if (!in.eof() && in.fail()) {
    std::cerr << "bad input\n";
    return 1;
  }
  return 0;
}
