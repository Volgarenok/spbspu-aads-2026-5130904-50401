#include "commands.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <utility>

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "Filename is required\n";
    return 1;
  }

  studilova::GraphsMap graphs(128);

  std::ifstream input(argv[1]);

  if (input)
  {
    std::string graphName;

    while (input >> graphName)
    {
      size_t edgesCount = 0;

      if (!(input >> edgesCount))
      {
        return 0;
      }

      studilova::Graph graph(edgesCount * 2 + 16);

      for (size_t i = 0; i < edgesCount; ++i)
      {
        std::string from;
        std::string to;
        size_t weight = 0;

        input >> from >> to >> weight;

        if (!input)
        {
          return 0;
        }

        graph.bind(from, to, weight);
      }

      graphs.insert(std::pair< std::string, studilova::Graph >(graphName, graph));
    }
  }

  studilova::CommandsMap commands(32);

  commands.insert(std::pair< std::string, studilova::Command >("graphs", studilova::graphs));
  commands.insert(std::pair< std::string, studilova::Command >("vertexes", studilova::vertexes));
  commands.insert(std::pair< std::string, studilova::Command >("outbound", studilova::outbound));
  commands.insert(std::pair< std::string, studilova::Command >("inbound", studilova::inbound));
  commands.insert(std::pair< std::string, studilova::Command >("bind", studilova::bind));
  commands.insert(std::pair< std::string, studilova::Command >("cut", studilova::cut));
  commands.insert(std::pair< std::string, studilova::Command >("create", studilova::create));
  commands.insert(std::pair< std::string, studilova::Command >("merge", studilova::merge));
  commands.insert(std::pair< std::string, studilova::Command >("extract", studilova::extract));

  std::string commandName;

  while (std::cin >> commandName)
  {
    if (!commands.contains(commandName))
    {
      std::cout << "<INVALID COMMAND>\n";

      std::string line;
      std::getline(std::cin, line);

      continue;
    }

    try
    {
      commands.at(commandName)(std::cin, std::cout, graphs);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
