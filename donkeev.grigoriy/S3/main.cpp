#include <iostream>
#include <fstream>
#include <string>
#include "Hash-table.hpp"
#include "hashTableIt.hpp"
#include "Graph.hpp"
#include "hash-structs.hpp"
#include "graphs-commands.hpp"

//(graphsHashTable_t, const std::string&, const std::string&, const size_t, 
//const topit::Vector< std::string >&, std::ostream&

namespace donkeev
{
  using graphsHashTable_t = donkeev::HashTable< std::string, donkeev::Graph, donkeev::GraphNameHash, donkeev::GraphEqual>;
  using commandFunc = void (*)(graphsHashTable_t, const std::string&, std::ostream&);
}

const topit::Vector< std::pair< std::string, donkeev::commandFunc > > commandsVector{
  std::initializer_list<std::pair<std::string, donkeev::commandFunc>>{
  {"graphs", donkeev::printGrapsNames},
  /*{"vertexes", donkeev::printVertexesNames},
  {"outbound", donkeev::printOutboundVertexesNames},
  {"inbound", donkeev::printInboundVertexesNames},
  {"bind", donkeev::createEdge},
  {"cut", donkeev::deleteEdge},
  {"create", donkeev::createGraph},
  {"merge", donkeev::mergeGraphs},
  {"extract", donkeev::extractGraph}*/
  }
};


std::string nextWord(const std::string& line, size_t& position)
{
  while (position < line.size() && line[position] == ' ')
  {
    ++position;
  }
  size_t start = position;
  while (position < line.size() && line[position] != ' ')
  {
    ++position;
  }

  return line.substr(start, position - start);
}
void readGraphs(const std::string& filename, donkeev::HashTable<std::string, donkeev::Graph, donkeev::GraphNameHash, donkeev::GraphEqual>& graphs)
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
    std::string graphName = nextWord(line, position);

    std::string edgesCountStr = nextWord(line, position);
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
      std::string from = nextWord(line, position);
      std::string to = nextWord(line, position);
      std::string weightStr = nextWord(line, position);

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

  donkeev::HashTable< std::string, donkeev::Graph, donkeev::GraphNameHash, donkeev::GraphEqual> graphsTable(16, 4);
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
    std::string command = nextWord(commandLine, readingPosition);

    if (commandLine.empty())
    {
      continue;
    }
    else if (command == "graphs")
    {
      donkeev::printGrapsNames(graphsTable, std::string(""),std::cout);
    }
    else
    {
      std::cout << "INVALID COMMAND" << '\n';
    }
  }
  
  donkeev::HashTable< std::string, void (*)(), donkeev::CommandsHash, donkeev::CommandsEqual > commands(16, 4);

}
