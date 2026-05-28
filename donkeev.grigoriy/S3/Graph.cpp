#include "Graph.hpp"

donkeev::Graph::Graph():
  edgesCount_(0),
  table_(1, 1),
  uniqueVertexes_()
{}

donkeev::Graph::Graph(const size_t bucketCount, const size_t bucketSize):
  edgesCount_(0),
  table_(bucketCount, bucketSize),
  uniqueVertexes_()
{}

void donkeev::Graph::addEdge(const std::string from, const std::string to, const size_t weight)
{
  if (!uniqueVertexes_.has(from))
  {
    uniqueVertexes_.pushBack(from);
  }
  if (!uniqueVertexes_.has(to))
  {
    uniqueVertexes_.pushBack(to);
  }
  
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

void donkeev::Graph::addVertex(const std::string& vertexName)
{
  uniqueVertexes_.pushBack(vertexName);
}
