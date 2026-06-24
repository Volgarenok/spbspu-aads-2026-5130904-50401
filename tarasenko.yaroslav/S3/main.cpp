#include <iostream>
#include <string>
#include <exception>
#include <fstream>
#include <limits>
#include "commands.hpp"
#include "graph.hpp"

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "1 argument expected\n";
    return 1;
  }

  std::ifstream input(argv[1]);
  if (!input)
  {
    std::cerr << "Failed to open file\n";
    return 1;
  }
  tarasenko::GraphTable graphs = tarasenko::getFromFile(input);
  tarasenko::HashTable< std::string, tarasenko::Command > cmds;
  cmds.add("graphs", tarasenko::showGraphs);
  cmds.add("vertexes", tarasenko::showVertexes);
  cmds.add("outbound", tarasenko::showOutbound);
  cmds.add("inbound", tarasenko::showInbound);
  cmds.add("bind", tarasenko::bind);
  cmds.add("cut", tarasenko::cut);
  cmds.add("create", tarasenko::createGraph);
  cmds.add("merge", tarasenko::mergeGraphs);
  cmds.add("extract", tarasenko::extractGraph);

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout, graphs);
      if (cmd == "graphs" || cmd == "vertexes" || cmd == "outbound" || cmd == "inbound")
      {
        std::cout << "\n";
      }
    }
    catch (const std::exception&)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::streamsize toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.clear();
      std::cin.ignore(toignore, '\n');
    }
  }
}
