#include <iostream>
#include <fstream>
#include <string>
#include "Hash-table.hpp"
#include "hashTableIt.hpp"
#include "Graph.hpp"
#include "hash-structs.hpp"
#include "graphs-commands.hpp"

namespace donkeev
{
  using graphsHashTable_t = donkeev::HashTable< std::string, donkeev::Graph, donkeev::GraphNameHash, donkeev::GraphEqual>;
  using commandFunc = void (*)(graphsHashTable_t, const std::string&, std::ostream&);
}

void readGraphs(const std::string& filename, donkeev::graphsHashTable_t& graphs)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Cannot open file");
  }

  std::string line;
  while (std::getline(file, line))
  {
    if (line.empty())
    {
      continue;
    }
    size_t position = 0;
    std::string graphName = donkeev::nextWord(line, position);

    std::string edgesCountStr = donkeev::nextWord(line, position);
    if (edgesCountStr.empty())
    {
      throw std::runtime_error("Bad input file");
    }
    size_t edgesCount = std::stoull(edgesCountStr);

    donkeev::Graph graph(16, 4);

    for (size_t i = 0; i < edgesCount; ++i)
    {
      do
      {
        std::getline(file, line);
      } while (line.empty());

      position = 0;
      std::string from = donkeev::nextWord(line, position);
      std::string to = donkeev::nextWord(line, position);
      std::string weightStr = donkeev::nextWord(line, position);

      if (from.empty() || to.empty() || weightStr.empty())
      {

        throw std::runtime_error("Bad input filee");
      }

      size_t weight = std::stoull(weightStr);
      graph.addEdge(from, to, weight);
    }

    graphs.add(graphName, graph);
  }
}
int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "There no filename\n";
    return 1;
  }
  std::string filename(argv[1]);

  donkeev::graphsHashTable_t graphsTable(16, 4);
  try
  {
    readGraphs(filename, graphsTable);
  }
  catch (const std::overflow_error& e)
  {
    std::cerr << e.what() << '\n';
  }
  catch (const std::runtime_error& e)
  {
    std::cerr << e.what() << '\n';
  }

  std::string commandLine;
  while (std::getline(std::cin, commandLine))
  {
    size_t readingPosition = 0;
    std::string command = donkeev::nextWord(commandLine, readingPosition);

    if (commandLine.empty())
    {
      continue;
    }
    else if (command == "graphs")
    {
      donkeev::printGrapsNames(graphsTable, std::string(""),std::cout);
    }
    else if (command == "vertexes")
    {
      std::string graphName = donkeev::nextWord(commandLine, readingPosition);
      if (graphName.empty())
      {
        std::cout << "INVALID COMMAND\n";
        continue;
      }
      donkeev::printVertexesNames(graphsTable, graphName, std::cout);
    }
    else if (command == "outbound")
    {
      std::string graphName = donkeev::nextWord(commandLine, readingPosition);
      std::string vertexName = donkeev::nextWord(commandLine, readingPosition);
      if (graphName.empty() || vertexName.empty())
      {
        std::cout << "INVALID COMMAND\n";
        continue;
      }
      std::string parametrs(graphName + " " + vertexName);
      try
      {
        donkeev::printOutboundVertexesNames(graphsTable, parametrs, std::cout);
      }
      catch (...)
      {
        std::cout << "INVALID COMMAND\n";
      }
    }
    else if (command == "inbound")
    {
      std::string graphName = donkeev::nextWord(commandLine, readingPosition);
      std::string vertexName = donkeev::nextWord(commandLine, readingPosition);
      if (graphName.empty() || vertexName.empty())
      {
        std::cout << "INVALID COMMAND\n";
        continue;
      }
      std::string parametrs(graphName + " " + vertexName);
      try
      {
        donkeev::printInboundVertexesNames(graphsTable, parametrs, std::cout);
      }
      catch (...)
      {
        std::cout << "INVALID COMMAND\n";
      }
    }
    else if (command == "bind")
    {
      std::string graphName = donkeev::nextWord(commandLine, readingPosition);
      std::string vertexFromName = donkeev::nextWord(commandLine, readingPosition);
      std::string vertexToName = donkeev::nextWord(commandLine, readingPosition);
      std::string weight = donkeev::nextWord(commandLine, readingPosition);
      if (graphName.empty() || vertexFromName.empty() || vertexToName.empty())
      {
        std::cout << "INVALID COMMAND\n";
        continue;
      }

      std::string parametrs(graphName + " " + vertexFromName + " " + vertexToName + " " + weight);
      try
      {
        donkeev::createEdge(graphsTable, parametrs, std::cout);
      }
      catch (...)
      {
        std::cout << "INVALID COMMAND\n";
      }
    }
    else if (command == "cut")
    {
      std::string graphName = donkeev::nextWord(commandLine, readingPosition);
      std::string vertexFromName = donkeev::nextWord(commandLine, readingPosition);
      std::string vertexToName = donkeev::nextWord(commandLine, readingPosition);
      std::string weight = donkeev::nextWord(commandLine, readingPosition);
      if (graphName.empty() || vertexFromName.empty() || vertexToName.empty())
      {
        std::cout << "INVALID COMMAND\n";
        continue;
      }

      std::string parametrs(graphName + " " + vertexFromName + " " + vertexToName + " " + weight);
      try
      {
        donkeev::deleteEdge(graphsTable, parametrs, std::cout);
      }
      catch (...)
      {
        std::cout << "INVALID COMMAND\n";
      }
    }
    else if (command == "create")
    {
      std::string graphName = donkeev::nextWord(commandLine, readingPosition);
      if (graphName.empty())
      {
        std::cout << "INVALID COMMAND\n";
        continue;
      }

      try
      {
        donkeev::createGraph(graphsTable, commandLine, std::cout);
      }
      catch (...)
      {
        std::cout << "INVALID COMMAND\n";
      }
    }
    else if (command == "merge")
    {
      std::string targetGraphName = donkeev::nextWord(commandLine, readingPosition);
      std::string firstGraphName = donkeev::nextWord(commandLine, readingPosition);
      std::string secondGraphName = donkeev::nextWord(commandLine, readingPosition);
      if (firstGraphName.empty() || secondGraphName.empty())
      {
        std::cout << "INVALID COMMAND\n";
        continue;
      }

      std::string parametrs(targetGraphName + " " + firstGraphName + " " + secondGraphName);
      try
      {
        donkeev::mergeGraphs(graphsTable, parametrs, std::cout);
      }
      catch (...)
      {
        std::cout << "INVALID COMMAND\n";
      }
    }
    else if (command == "extract")
    {
      std::string targetGraphName = donkeev::nextWord(commandLine, readingPosition);
      std::string templateGraphName = donkeev::nextWord(commandLine, readingPosition);
      if (targetGraphName.empty() || templateGraphName.empty())
      {
        std::cout << "INVALID COMMAND\n";
        continue;
      }

      try
      {
        donkeev::extractGraph(graphsTable, commandLine, std::cout);
      }
      catch (...)
      {
        std::cout << "INVALID COMMAND\n";
      }
    }
    else
    {
      std::cout << "INVALID COMMAND" << '\n';
    }
  }

  donkeev::HashTable< std::string, void (*)(), donkeev::CommandsHash, donkeev::CommandsEqual > commands(16, 4);

}

