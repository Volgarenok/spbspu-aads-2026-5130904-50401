#include "graphs-commands.hpp"

std::string donkeev::nextWord(const std::string& line, size_t& position)
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

void donkeev::sortNames(topit::Vector< std::string >& vector)
{
  for (size_t i = 0; i < vector.getSize(); ++i)
  {
    size_t minId = i;
    for (size_t j = i + 1; j < vector.getSize(); ++j)
    {
      if (vector[j] < vector[minId])
      {
        minId = j;
      }
    }

    std::swap(vector[i], vector[minId]);
  }
}

void donkeev::printGrapsNames(graphsHashTable_t graphsTable, const std::string&, std::ostream& out)
{
  topit::Vector< std::string > sortedVector;

  donkeev::HTIt< std::string, donkeev::Graph > begin = graphsTable.begin();
  donkeev::HTIt< std::string, donkeev::Graph > end = graphsTable.end();
  for (; begin != end; ++begin)
  {
    sortedVector.pushBack((*begin).first);
  }

  sortNames(sortedVector);
  for (size_t i = 0; i < sortedVector.getSize(); ++i)
  {
    out << sortedVector[i] << '\n';
  }
}

void donkeev::printVertexesNames(graphsHashTable_t graphsTable, const std::string& parametrs, std::ostream& out)
{
  std::string graphName(parametrs);
  topit::Vector< std::string > sortedVector;

  donkeev::Graph* graph_ptr = graphsTable.find(graphName);
  if (graph_ptr == nullptr)
  {
    std::cout << "INVALID COMMAND\n"; 
    throw std::runtime_error("Bad input");
  }

  donkeev::Graph graph = *graph_ptr;
  donkeev::HashTable< std::pair< std::string, std::string >, donkeev::List< size_t >, donkeev::VertexPairHash, donkeev::VertexPairEqual > graphHT = graph.table_; 
  donkeev::HTIt< std::pair< std::string, std::string >, donkeev::List< size_t > > begin = graphHT.begin();
  donkeev::HTIt< std::pair< std::string, std::string >, donkeev::List< size_t > > end = graphHT.end();

  for (; begin != end; ++begin)
  {
    std::pair< std::pair< std::string, std::string >, donkeev::List< size_t > > graphPair = *begin;
    if (!sortedVector.has(graphPair.first.first))
    {
      sortedVector.pushBack(graphPair.first.first);
    }
    if (!sortedVector.has(graphPair.first.second))
    {
      sortedVector.pushBack(graphPair.first.second);
    }
  }

  sortNames(sortedVector);
  for (size_t i = 0; i < sortedVector.getSize(); ++i)
  {
    out << sortedVector[i] << '\n';
  }
}
