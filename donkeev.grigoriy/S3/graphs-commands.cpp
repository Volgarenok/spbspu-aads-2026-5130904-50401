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

void donkeev::sortNames(topit::Vector< std::pair< std::string, donkeev::List< size_t > > >& vector)
{
  for (size_t i = 0; i < vector.getSize(); ++i)
  {
    size_t minId = i;
    for (size_t j = i + 1; j < vector.getSize(); ++j)
    {
      if (vector[j].first < vector[minId].first)
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

void donkeev::printGrapsNames(graphsHashTable_t& graphsTable, const std::string&, std::ostream& out)
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

void donkeev::printVertexesNames(graphsHashTable_t& graphsTable, const std::string& parametrs, std::ostream& out)
{
  std::string graphName(parametrs);

  donkeev::Graph* graph_ptr = graphsTable.find(graphName);
  if (graph_ptr == nullptr)
  {
    std::cout << "INVALID COMMAND\n"; 
    throw std::runtime_error("Bad input");
  }

  donkeev::Graph& graph = *graph_ptr;
  topit::Vector< std::string > sortedVector(graph.uniqueVertexes_);
  sortNames(sortedVector);
  for (size_t i = 0; i < sortedVector.getSize(); ++i)
  {
    out << sortedVector[i] << '\n';
  }
}

void donkeev::printOutboundVertexesNames(graphsHashTable_t& graphsTable, const std::string& parametrs, std::ostream& out)
{
  size_t readingPosition = 0;
  std::string graphName = donkeev::nextWord(parametrs, readingPosition);
  std::string vertexName = donkeev::nextWord(parametrs, readingPosition);

  donkeev::Graph* graph_ptr = graphsTable.find(graphName);
  if (graph_ptr == nullptr)
  {
    throw std::runtime_error("Bad input");
  }

  donkeev::Graph& graph = *graph_ptr; 
  donkeev::HTIt< std::pair< std::string, std::string >, donkeev::List< size_t > > HTbegin = graph.table_.begin();
  donkeev::HTIt< std::pair< std::string, std::string >, donkeev::List< size_t > > HTend = graph.table_.end();

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

void donkeev::printInboundVertexesNames(graphsHashTable_t& graphsTable, const std::string& parametrs, std::ostream& out)
{
  size_t readingPosition = 0;
  std::string graphName = donkeev::nextWord(parametrs, readingPosition);
  std::string vertexName = donkeev::nextWord(parametrs, readingPosition);

  donkeev::Graph* graph_ptr = graphsTable.find(graphName);
  if (graph_ptr == nullptr)
  {
    throw std::runtime_error("Bad input");
  }

  donkeev::Graph& graph = *graph_ptr;

  if (!graph.uniqueVertexes_.has(vertexName))
  {
    throw std::runtime_error("No such vertex");
  }

  donkeev::HTIt< std::pair< std::string, std::string >, donkeev::List< size_t > > graphHTBegin = graph.table_.begin();
  donkeev::HTIt< std::pair< std::string, std::string >, donkeev::List< size_t > > graphHTEnd = graph.table_.end();
  
  topit::Vector< std::pair< std::string, donkeev::List< size_t > > > sortedInboundVertexesNames;
  while (graphHTBegin != graphHTEnd)
  {
    if ((*graphHTBegin).first.second == vertexName)
    {
      std::string inboundVertex = (*graphHTBegin).first.first;
      donkeev::List< size_t >& inboundWeights = (*graphHTBegin).second;
      sortedInboundVertexesNames.pushBack({inboundVertex, inboundWeights});
    }

    ++graphHTBegin;
  }

  sortNames(sortedInboundVertexesNames);

  if (sortedInboundVertexesNames.isEmpty())
  {
    out << '\n';
    return;
  }

  for (size_t i = 0; i < sortedInboundVertexesNames.getSize(); ++i)
  {
    out << sortedInboundVertexesNames[i].first << " ";

    topit::Vector< size_t > sortedWeights;
    donkeev::List< size_t >& weightsList = sortedInboundVertexesNames[i].second;
    donkeev::LIter< size_t > weightsListBegin = weightsList.begin();
    for (size_t j = 0; j < weightsList.size(); ++j)
    {
      sortedWeights.pushBack(*weightsListBegin);
      ++weightsListBegin;
    }

    sortNumbers(sortedWeights);

    size_t j = 0;
    for (; j < sortedWeights.getSize() - 1; ++j)
    {
      out << sortedWeights[j] << " ";
    }
    out << sortedWeights[j] << '\n';
  }
}

void donkeev::createEdge(graphsHashTable_t& graphsTable, const std::string& parametrs, std::ostream&)
{
  size_t readingPosition = 0;
  std::string graphName = donkeev::nextWord(parametrs, readingPosition);
  std::string vertexFromName = donkeev::nextWord(parametrs, readingPosition);
  std::string vertexToName = donkeev::nextWord(parametrs, readingPosition);
  size_t weight = std::stoull(donkeev::nextWord(parametrs, readingPosition));

  donkeev::Graph* graph_ptr = graphsTable.find(graphName);
  if (graph_ptr == nullptr)
  {
    throw std::runtime_error("Bad input");
  }
  graph_ptr->addEdge(vertexFromName, vertexToName, weight);

}

void donkeev::deleteEdge(graphsHashTable_t& graphsTable, const std::string& parametrs, std::ostream&)
{
  size_t readingPosition = 0;
  std::string graphName = donkeev::nextWord(parametrs, readingPosition);
  std::string vertexFromName = donkeev::nextWord(parametrs, readingPosition);
  std::string vertexToName = donkeev::nextWord(parametrs, readingPosition);
  size_t weight = std::stoull(donkeev::nextWord(parametrs, readingPosition));

  donkeev::Graph* graph_ptr = graphsTable.find(graphName);
  if (graph_ptr == nullptr)
  {
    throw std::runtime_error("Bad input");
  }
  graph_ptr->deleteEdge(vertexFromName, vertexToName, weight);
}

void donkeev::createGraph(graphsHashTable_t& graphsTable, const std::string& parametrs, std::ostream&)
{
  size_t readingPosition = 0;
  donkeev::nextWord(parametrs, readingPosition);
  std::string graphName = donkeev::nextWord(parametrs, readingPosition);
  std::string countStr = donkeev::nextWord(parametrs, readingPosition);
  
  donkeev::Graph* graph_ptr = graphsTable.find(graphName);
  if (graph_ptr != nullptr)
  {
    throw std::runtime_error("Bad input");
  }

  donkeev::Graph thisGraph(16, 4);
  if (countStr.empty() || countStr == "0")
  {
    graphsTable.add(graphName, thisGraph);
    return;
  }

  size_t count = std::stoull(countStr);
  for (size_t i = 0; i < count; ++i)
  {
    std::string vertexName = donkeev::nextWord(parametrs, readingPosition);
    if (vertexName.empty())
    {
      throw std::runtime_error("Bad input");
    }
    thisGraph.uniqueVertexes_.pushBack(vertexName);
  }
  std::string extra = donkeev::nextWord(parametrs, readingPosition);
  if (!extra.empty())
  {
    throw std::runtime_error("Bad input");
  }

  graphsTable.add(graphName, std::move(thisGraph));
}

void donkeev::mergeGraphs(graphsHashTable_t& graphsTable, const std::string& parametrs, std::ostream&)
{
  size_t readingPosition = 0;
  std::string targetGraphName = donkeev::nextWord(parametrs, readingPosition);
  std::string firstGraphName = donkeev::nextWord(parametrs, readingPosition);
  std::string secondGraphName = donkeev::nextWord(parametrs, readingPosition);

  if (graphsTable.has(targetGraphName))
  {
    throw std::runtime_error("Bad input");
  }

  donkeev::Graph* firstGraph_ptr = graphsTable.find(firstGraphName);
  donkeev::Graph* secondGraph_ptr = graphsTable.find(secondGraphName);
  if (firstGraph_ptr == nullptr || secondGraph_ptr == nullptr)
  {
    throw std::runtime_error("Bad input");
  }

  donkeev::Graph thisGraph(16, 4);
  donkeev::HTIt< std::pair< std::string, std::string >, donkeev::List< size_t > > firstBegin = firstGraph_ptr->table_.begin();
  for (size_t i = 0; i < firstGraph_ptr->table_.size(); ++i)
  {
    donkeev::LIter< size_t > pairListIt = (*firstBegin).second.begin();
    for (size_t j = 0; j < (*firstBegin).second.size(); ++j)
    {
      thisGraph.addEdge((*firstBegin).first.first, (*firstBegin).first.second, *pairListIt);
      ++pairListIt;
    }
    ++firstBegin;
  }
  donkeev::HTIt< std::pair< std::string, std::string >, donkeev::List< size_t > > secondBegin = secondGraph_ptr->table_.begin();
  for (size_t i = 0; i < secondGraph_ptr->table_.size(); ++i)
  {
    donkeev::LIter< size_t > pairListIt = (*secondBegin).second.begin();
    for (size_t j = 0; j < (*secondBegin).second.size(); ++j)
    {
      thisGraph.addEdge((*secondBegin).first.first, (*secondBegin).first.second, *pairListIt);
      ++pairListIt;
    }
    ++secondBegin;
  }

  topit::Vector< std::string >& firstVector = firstGraph_ptr->uniqueVertexes_;
  topit::Vector< std::string >& secondVector = secondGraph_ptr->uniqueVertexes_;
  for (size_t i = 0; i < firstVector.getSize(); ++i)
  {
    if (!thisGraph.uniqueVertexes_.has(firstVector[i]))
    {
      thisGraph.uniqueVertexes_.pushBack(firstVector[i]);
    }
  }
  for (size_t i = 0; i < secondVector.getSize(); ++i)
  {
    if (!thisGraph.uniqueVertexes_.has(secondVector[i]))
    {
      thisGraph.uniqueVertexes_.pushBack(secondVector[i]);
    }
  }

  graphsTable.add(targetGraphName, std::move(thisGraph));
}

void donkeev::extractGraph(graphsHashTable_t& graphsTable, const std::string& parametrs, std::ostream&)
{
  size_t readingPosition = 0;
  donkeev::nextWord(parametrs, readingPosition);
  std::string targetGraphName = donkeev::nextWord(parametrs, readingPosition);
  std::string templateGraphName = donkeev::nextWord(parametrs, readingPosition);
  std::string countStr = donkeev::nextWord(parametrs, readingPosition);

  if (graphsTable.has(targetGraphName))
  {
    throw std::runtime_error("Bad input");
  }
  
  donkeev::Graph* templateGraph_ptr = graphsTable.find(templateGraphName);
  if (templateGraph_ptr == nullptr)
  {
    throw std::runtime_error("Bad input");
  }
  
  donkeev::Graph thisGraph(16, 4);
  if (countStr.empty() || countStr == "0")
  {
    graphsTable.add(targetGraphName, std::move(thisGraph));
    return;
  }
  
  size_t count = std::stoull(countStr);
  topit::Vector< std::string > vertexes;
  for (size_t i = 0; i < count; ++i)
  {
    std::string vertex = donkeev::nextWord(parametrs, readingPosition);
    if (vertex.empty() || !templateGraph_ptr->uniqueVertexes_.has(vertex))
    {
      throw std::runtime_error("Bad input");
    }
    vertexes.pushBack(vertex);
  }
  
  donkeev::HTIt< std::pair< std::string, std::string >, donkeev::List< size_t > > templateIt = templateGraph_ptr->table_.begin();
  for (size_t i = 0; i < templateGraph_ptr->table_.size(); ++i)
  {
    if (vertexes.has((*templateIt).first.first) && vertexes.has((*templateIt).first.second))
    {
      std::string firstVert = (*templateIt).first.first;
      std::string secondVer = (*templateIt).first.second;
      donkeev::LIter< size_t > it = (*templateIt).second.begin();
      for (size_t j = 0; j < (*templateIt).second.size(); ++j)
      {
        thisGraph.addEdge(firstVert, secondVer, *it);
        ++it;
      }
      ++templateIt;
    }
  }

  if (thisGraph.uniqueVertexes_.getSize() != vertexes.getSize())
  {
    for (size_t i = 0; i < vertexes.getSize(); ++i)
    {
      if (!thisGraph.uniqueVertexes_.has(vertexes[i]))
      {
        thisGraph.uniqueVertexes_.pushBack(vertexes[i]);
      }
    }
  }

  graphsTable.add(targetGraphName, std::move(thisGraph));
}
