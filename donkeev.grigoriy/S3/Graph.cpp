#include "Graph.hpp"

donkeev::Graph::Graph():
  table_(1, 1),
  uniqueVertexes_()
{}

donkeev::Graph::Graph(const size_t bucketCount, const size_t bucketSize):
  table_(bucketCount, bucketSize),
  uniqueVertexes_()
{}

donkeev::Graph::Graph(const Graph& other):
  table_(other.table_),
  uniqueVertexes_(other.uniqueVertexes_)
{}

donkeev::Graph::Graph(Graph&& other) noexcept:
  table_(std::move(other.table_)),
  uniqueVertexes_(std::move(other.uniqueVertexes_))
{}

donkeev::Graph& donkeev::Graph::operator=(const Graph& other)
{
  if (this == &other) return *this;

  table_ = other.table_;
  uniqueVertexes_ = other.uniqueVertexes_;

  return *this;
}

donkeev::Graph& donkeev::Graph::operator=(Graph&& other) noexcept
{
  if (this == &other) return *this;

  table_ = std::move(other.table_);
  uniqueVertexes_ = std::move(other.uniqueVertexes_);

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
  
  edgesPair_t thisKey = std::make_pair(from, to);
  donkeev::List< size_t >* thisValue = table_.find(thisKey);
  if (thisValue == nullptr)
  {
    donkeev::List< size_t > weights;
    weights.pushBack(weight);

    table_.add(thisKey, weights);
  
    return;
  }
  
  thisValue->pushBack(weight);
}

void donkeev::Graph::addVertex(const std::string& vertexName)
{
  uniqueVertexes_.pushBack(vertexName);
}

void donkeev::Graph::deleteEdge(const std::string from, const std::string to, const size_t weight)
{
  edgesPair_t thisKey = std::make_pair(from, to);
  donkeev::List< size_t >* thisValue_ptr = table_.find(thisKey);
  if (thisValue_ptr == nullptr)
  {
    throw std::runtime_error("Bad input");
  }

  donkeev::List< size_t >& thisValue = *thisValue_ptr;
  if(thisValue.has(weight))
  {
    thisValue.deleteNode(weight);
  }
  else
  {
    throw std::runtime_error("Bad input");
  }
  
  if (thisValue.size() == 0)
  {
    table_.drop(thisKey);
  }
  bool hasYAFrom = false;
  bool hasYATo = false;
  EdgeTable_t::iterator begin = table_.begin();
  EdgeTable_t::iterator end = table_.end();
  while (begin != end)
  {
    if ((*begin).first.first == from || (*begin).first.second == from)
    {
      hasYAFrom = true;
    }
    if ((*begin).first.first == to || (*begin).first.second == to)
    {
      hasYATo = true;
    }

    ++begin;
  }

  if (!hasYAFrom)
  {
    uniqueVertexes_.erase(uniqueVertexes_.findId(from));
  }
  if (!hasYATo)
  {
    uniqueVertexes_.erase(uniqueVertexes_.findId(to));
  }
}
