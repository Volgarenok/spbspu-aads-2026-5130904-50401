#include "commands.hpp"

#include <fstream>
#include <iostream>
#include <string>

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

      graphs.add(graphName, graph);
    }
  }

  studilova::CommandsMap commands(32);
  studilova::initCommands(commands);

  std::string commandName;

  while (std::cin >> commandName)
  {
    if (!commands.has(commandName))
    {
      std::cout << "<INVALID COMMAND>\n";

      std::string line;
      std::getline(std::cin, line);

      continue;
    }

    try
    {
      commands.get(commandName)(std::cin, std::cout, graphs);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    std::cin.clear();
    std::string rest;
    std::getline(std::cin, rest);
  }

  return 0;
}
