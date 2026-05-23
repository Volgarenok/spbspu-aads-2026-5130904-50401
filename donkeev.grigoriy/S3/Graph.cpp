#include "Graph.hpp"

donkeev::Graph::Graph(const size_t bucketCount, const size_t bucketSize):
  edgesCount_(0),
  table_(bucketCount, bucketSize)
{}

void donkeev::Graph::addEdge(const std::string from, const std::string to, const size_t weight)
{
  std::pair< std::string, std::string > thisKey = std::make_pair(from, to);
  donkeev::List< size_t >* thisValue = table_.find(thisKey);
  if (thisValue == nullptr)
  {
    donkeev::List< size_t > weights;
    weights.pushBack(weight);

    table_.add(std::make_pair(from, to), std::move(weights));
    return;
  }
  
  thisValue->pushBack(weight);
}
