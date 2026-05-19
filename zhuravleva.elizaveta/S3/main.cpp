#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include "commands.hpp"
#include "graph.hpp"
#include "hasher.hpp"
#include "myHash.hpp"

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    std::cerr << "Not enough arguments\n";
    return 1;
  }
  std::ifstream input(argv[1]);
  if (!input)
  {
    std::cerr << "Cannot open file\n";
    return 1;
  }
  zhuravleva::GraphTable graphs;
  std::string graphName;
  size_t edgeCount = 0;
  while (input >> graphName >> edgeCount)
  {
    zhuravleva::Graph graph;
    for (size_t i = 0; i < edgeCount; i++)
    {
      std::string from;
      std::string to;
      size_t weight = 0;
      input >> from >> to >> weight;
      graph.bind(from, to, weight);
    }
    graphs.add(graphName, graph);
  }

  typedef void (*Command)(std::ostream&, std::istream&, zhuravleva::GraphTable&);
  typedef void (*ConstCommand)(std::ostream&, std::istream&, const zhuravleva::GraphTable&);
  zhuravleva::HashTable< std::string, Command, zhuravleva::Blake2Hasher< std::string >, zhuravleva::KeyEqual > commands;
  zhuravleva::HashTable< std::string, ConstCommand, zhuravleva::Blake2Hasher< std::string >, zhuravleva::KeyEqual > constCommands;

  constCommands.add("graphs", zhuravleva::graphs);
  constCommands.add("vertexes", zhuravleva::vertexes);
  constCommands.add("outbound", zhuravleva::outbound);
  constCommands.add("inbound", zhuravleva::inbound);
  commands.add("bind", zhuravleva::bind);
  commands.add("cut", zhuravleva::cut);
  commands.add("create", zhuravleva::create);
  commands.add("merge", zhuravleva::merge);
  commands.add("extract", zhuravleva::extract);
  std::string commandName;

  while (std::cin >> commandName)
  {
    try
    {
      if (commands.has(commandName))
      {
        commands.get(commandName)(std::cout, std::cin, graphs);
      }
      else if (constCommands.has(commandName))
      {
        constCommands.get(commandName)(std::cout, std::cin, graphs);
      }
      else
      {
        throw std::runtime_error("invalid command");
      }
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
