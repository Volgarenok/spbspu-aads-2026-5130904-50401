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

void donkeev::sortNumbers(topit::Vector< size_t >& vector)
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

void donkeev::printOutboundVertexesNames(graphsHashTable_t graphsTable, const std::string& parametrs, std::ostream& out)
{
  size_t readingPosition = 0;
  std::string graphName = donkeev::nextWord(parametrs, readingPosition);
  std::string vertexName = donkeev::nextWord(parametrs, readingPosition);

  donkeev::Graph* graph_ptr = graphsTable.find(graphName);
  if (graph_ptr == nullptr)
  {
    throw std::runtime_error("Bad input");
  }

  donkeev::Graph graph = *graph_ptr;
  donkeev::HashTable< std::pair< std::string, std::string >, donkeev::List< size_t >, donkeev::VertexPairHash, donkeev::VertexPairEqual > graphHT = graph.table_; 
  donkeev::HTIt< std::pair< std::string, std::string >, donkeev::List< size_t > > HTbegin = graphHT.begin();
  donkeev::HTIt< std::pair< std::string, std::string >, donkeev::List< size_t > > HTend = graphHT.end();

  topit::Vector< std::pair< std::string, donkeev::List< size_t > > > outboundVertexes;
  for (; HTbegin != HTend; ++HTbegin)
  {
    std::pair< std::pair< std::string, std::string >, donkeev::List< size_t > > edgePair = *HTbegin; 
    if (edgePair.first.first == vertexName)
    {
      outboundVertexes.pushBack(std::make_pair(edgePair.first.second, edgePair.second));
    }
  }

  if (outboundVertexes.isEmpty())
  {
    for (size_t i = 0; i < graph.uniqueVertexes_.getSize(); ++i)
    {
      if (graph.uniqueVertexes_[i] == vertexName)
      {
        out << '\n';
        return;
      }
    }
    
    throw std::runtime_error("No such vertex");
  }

  topit::Vector< std::string > sortedVertexes;
  topit::VIter< std::pair< std::string, donkeev::List< size_t > > > Ebegin = outboundVertexes.begin();
  topit::VIter< std::pair< std::string, donkeev::List< size_t > > > Eend = outboundVertexes.end();
  while (Ebegin != Eend)
  {
    sortedVertexes.pushBack((*Ebegin).first);
    ++Ebegin;
  }

  sortNames(sortedVertexes);

  topit::VIter< std::string > Vbegin = sortedVertexes.begin();
  topit::VIter< std::string > Vend = sortedVertexes.end();
  while (Vbegin != Vend)
  {
    for (size_t i = 0; i < outboundVertexes.getSize(); ++i)
    {
      if (*Vbegin == outboundVertexes[i].first)
      {
        out << *Vbegin << " ";

        topit::Vector< size_t > sortedWeights;
        donkeev::LIter< size_t > Wbegin = outboundVertexes[i].second.begin();
        for (size_t j = 0; j < outboundVertexes[i].second.size(); ++j)
        {
          sortedWeights.pushBack(*Wbegin);
          ++Wbegin;
        }

        sortNumbers(sortedWeights);
        for (size_t j = 0; j < sortedWeights.getSize() - 1; ++j)
        {
          out << sortedWeights[j] << " ";
        }
        out << sortedWeights[sortedWeights.getSize() - 1] << '\n';
      }
    }
    ++Vbegin;
  }
}
