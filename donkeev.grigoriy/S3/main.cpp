#include <iostream>
#include <fstream>
#include <string>
#include "Hash-table.hpp"
#include "hashTableIt.hpp"
#include "Graph.hpp"
#include "hash-structs.hpp"

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
  readGraphs(filename, graphsTable);
}
