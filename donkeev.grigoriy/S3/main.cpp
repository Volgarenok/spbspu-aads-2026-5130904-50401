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
  using commandFunc = void (*)(graphsHashTable_t&, const std::string&, std::ostream&);
  using commandsTable_t = donkeev::HashTable< std::string, commandFunc, donkeev::CommandsHash, donkeev::CommandsEqual>;
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

  donkeev::commandsTable_t cmdTable(16, 4);
  cmdTable.add("graphs", donkeev::printGrapsNames);
  cmdTable.add("vertexes", donkeev::printVertexesNames);
  cmdTable.add("outbound", donkeev::printOutboundVertexesNames);
  cmdTable.add("inbound", donkeev::printInboundVertexesNames);
  cmdTable.add("bind", donkeev::createEdge);
  cmdTable.add("cut", donkeev::deleteEdge);
  cmdTable.add("create", donkeev::createGraph);
  cmdTable.add("merge", donkeev::mergeGraphs);
  cmdTable.add("extract", donkeev::extractGraph);

  std::string commandLine;
  while (std::getline(std::cin, commandLine))
  {
    size_t readingPosition = 0;
    std::string command = donkeev::nextWord(commandLine, readingPosition);

    if (commandLine.empty())
    {
      continue;
    }

    std::string argc = (readingPosition < commandLine.size() ? commandLine.substr(readingPosition): "");
    donkeev::commandFunc* func_ptr = cmdTable.find(command);
    if (func_ptr)
    {
      try
      {
        (*func_ptr)(graphsTable, argc, std::cout);
      }
      catch (...)
      {
        std::cout << "<INVALID COMMAND>\n";
      }
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}

