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

donkeev::Graph::Graph(const Graph& other):
  edgesCount_(other.edgesCount_),
  table_(other.table_),
  uniqueVertexes_(other.uniqueVertexes_)
{}

donkeev::Graph::Graph(Graph&& other) noexcept:
  edgesCount_(other.edgesCount_),
  table_(std::move(other.table_)),
  uniqueVertexes_(std::move(other.uniqueVertexes_))
{
  other.edgesCount_ = 0;
}

donkeev::Graph& donkeev::Graph::operator=(const Graph& other)
{
  if (this == &other) return *this;

  edgesCount_ = other.edgesCount_;
  table_ = other.table_;
  uniqueVertexes_ = other.uniqueVertexes_;

  return *this;
}

donkeev::Graph& donkeev::Graph::operator=(Graph&& other) noexcept
{
  if (this == &other) return *this;

  edgesCount_ = other.edgesCount_;
  table_ = std::move(other.table_);
  uniqueVertexes_ = std::move(other.uniqueVertexes_);
  other.edgesCount_ = 0;
  
  return *this;
}

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
