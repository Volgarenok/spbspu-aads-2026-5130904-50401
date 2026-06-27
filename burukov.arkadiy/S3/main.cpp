#include "commands.hpp"

#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>

#include <vector.hpp>

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <graph_file>\n";
    return 1;
  }
  std::ifstream inputFile(argv[1]);
  if (!inputFile.is_open())
  {
    std::cerr << "Cannot open file\n";
    return 1;
  }
  burukov::GraphDatabase database(16);
  std::string graphName;
  size_t edgeCount = 0;
  while (inputFile >> graphName >> edgeCount)
  {
    burukov::Graph newGraph;
    for (size_t i = 0; i < edgeCount; ++i)
    {
      std::string from;
      std::string to;
      size_t weight = 0;
      inputFile >> from >> to >> weight;
      if (!inputFile)
      {
        std::cerr << "Invalid graph description\n";
        return 2;
      }
      newGraph.addEdge(from, to, weight);
    }
    database.add(graphName, std::move(newGraph));
  }
  inputFile.close();

  using CommandFunction = void (*)(std::istream &, std::ostream &, burukov::GraphDatabase &);
  using CommandTable =
    burukov::HashTable< std::string, CommandFunction, burukov::SipHash< std::string >, std::equal_to< std::string > >;
  CommandTable commandTable(16);

  commandTable.add("graphs", burukov::commandGraphs);
  commandTable.add("vertexes", burukov::commandVertexes);
  commandTable.add("outbound", burukov::commandOutbound);
  commandTable.add("inbound", burukov::commandInbound);
  commandTable.add("bind", burukov::commandBind);
  commandTable.add("cut", burukov::commandCut);
  commandTable.add("create", burukov::commandCreate);
  commandTable.add("merge", burukov::commandMerge);
  commandTable.add("extract", burukov::commandExtract);

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      commandTable.at(command)(std::cin, std::cout, database);
    }
    catch (const std::exception &)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
